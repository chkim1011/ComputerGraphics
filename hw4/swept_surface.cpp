
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <GL/glut.h>

using namespace std;

static unsigned int width = 1000;
static unsigned int height = 1000;

static bool mouseRotatePressed = false;
static bool mouseMovePressed   = false;
static bool mouseZoomPressed   = false;
static bool mouseDollyPressed   = false;
static bool mouseSeekPressed   = false;
static bool fullScreen = false;

static float lookat_x = 0.0,lookat_y = 0.0, lookat_z = 0.0;
static float lastlookat_x = 0.0, lastlookat_y = 0.0, lastlookat_z = 0.0;
static float lastx =0.0, lasty = 0.0;
static float cam_x = 0.0,cam_y= 0.0, cam_z = 1.0;
static float vx = 0.0, vy = 1.0, vz = 0.0;
static float lastvx =0.0, lastvy = 0.0, lastvz = 0.0;
static float fov = 45.0, lastfov = 45.0;
static float cam_dist = 100.0, lastcam_dist = 100.0;


void normalize(float *a)
{
    float len =  sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
    a[0] /= len; a[1] /= len; a[2] /= len;
    return; 
}
void crossproduct(const float a[],const float b[], float c[])
{
    c[0] = a[1]*b[2] - a[2]*b[1];
    c[1] = -a[0]*b[2] + a[2]*b[0];
    c[2] = a[0]*b[1] - a[1]*b[0];
    return;
}
float innerproduct(const float a[],const float b[])
{
    return (a[0]*b[0]+a[1]*b[1]+a[2]*b[2]);
}

struct Quaternion {
    
    float w;
    float v[3];

};
struct Point 
{   
    Point(float x = 0, float y = 0, float z = 0) : x{x}, y{y}, z{z} {}

    float x; 
    float y;
    float z;
};

void normalize(Quaternion &p)
{
    float len = sqrt(p.w*p.w + p.v[0]*p.v[0] + p.v[1]*p.v[1] + p.v[2]*p.v[2]);
    p.w /= len; p.v[0] /= len; p.v[1] /= len; p.v[2] /= len;   
    return;
}
Quaternion qproduct(const Quaternion p, const Quaternion q)
{
    Quaternion res;
    float c[3];
    res.w = p.w*q.w - innerproduct(p.v,q.v);
    crossproduct(p.v,q.v,c);
    res.v[0] = p.w*q.v[0] + q.w*p.v[0] + c[0];
    res.v[1] = p.w*q.v[1] + q.w*p.v[1] + c[1];
    res.v[2] = p.w*q.v[2] + q.w*p.v[2] + c[2];

    return res;
}
Quaternion qinverse(Quaternion &p)
{
    p.v[0] *= -1;
    p.v[1] *= -1;
    p.v[2] *= -1;

    return p;
}
Quaternion qrotate(const Quaternion orient, const Quaternion rotate)
{
    Quaternion res;
    res.w = 0;
    float cross1[3];
    float cross2[3];

    crossproduct(rotate.v,orient.v,cross1);
    crossproduct(rotate.v,cross1,cross2);

    res.v[0] = rotate.w*rotate.w*orient.v[0] + innerproduct(orient.v,rotate.v)*rotate.v[0] + 2*rotate.w*cross1[0] + cross2[0];
    res.v[1] = rotate.w*rotate.w*orient.v[1] + innerproduct(orient.v,rotate.v)*rotate.v[1] + 2*rotate.w*cross1[1] + cross2[1];
    res.v[2] = rotate.w*rotate.w*orient.v[2] + innerproduct(orient.v,rotate.v)*rotate.v[2] + 2*rotate.w*cross1[2] + cross2[2];

    return res;
}
Quaternion qrotate_M(const Quaternion orient, const Quaternion rotate)
{
    float q0 = rotate.w; float q1 = rotate.v[0]; float q2 = rotate.v[1]; float q3 = rotate.v[2];
    float x = orient.v[0];
    float y = orient.v[1];
    float z = orient.v[2];

    float new_x = (q0*q0 + q1*q1 - q2*q2 - q3*q3)*x + (2*q1*q2 - 2*q0*q3)*y + (2*q1*q3 + 2*q0*q2)*z;
    float new_y = (2*q1*q2 + 2*q0*q3)*x + (q0*q0 - q1*q1 + q2*q2 - q3*q3)*y + (2*q2*q3 - 2*q0*q1)*z;
    float new_z = (2*q1*q3 - 2*q0*q2)*x + (2*q2*q3 + 2*q0*q1)*y + (q0*q0 - q1*q1 - q2*q2 + q3*q3)*z;

    Quaternion res{0,{new_x, new_y, new_z}};
    return res;
}
Quaternion exp(Quaternion vector)
{
    float x = vector.v[0];
    float y = vector.v[1];
    float z = vector.v[2];
    float a;

    float theta = sqrt(x*x + y*y + z*z);
    if ( theta < 0.0001) { a = 1; }
    else { sin(theta)/theta;}

    Quaternion res{cos(theta),{a*x, a*y, a*z}};
    
    return res;

}
Quaternion log(Quaternion quat)
{
    Quaternion vector{0,{0,0,0}};

    float sine= sqrt(quat.v[0]*quat.v[0]+quat.v[1]*quat.v[1]+quat.v[2]*quat.v[2]);
    float theta = atan2(sine, quat.w);
    float a;

    if (sine < 0.0001) { a = 1; }
    else { a = theta / sine; }
    
    vector.v[0] = a * quat.v[0];
    vector.v[1] = a * quat.v[1];
    vector.v[2] = a * quat.v[2];

    return vector;

}
Quaternion slerp(const Quaternion q0, const Quaternion q1, const float t)
{
    Quaternion res;
    float cosine = q0.w*q1.w + q0.v[0]*q1.v[0] +q0.v[1]*q1.v[1] +q0.v[2]*q1.v[2];
    float t_ = 1-t;

    if ( cosine > -1 + 0.0001)
    {
        if ( cosine < 1 - 0.0001)
        {
            float theta = acos(cosine);
            float sine = sin(theta);
            float a = sin(t_*theta)/sine;
            float b = sin(t*theta)/sine;
            res.w = a*q0.w + b*q1.w;
            res.v[0] = a*q0.v[0] + b*q1.v[0];
            res.v[1] = a*q0.v[1] + b*q1.v[1];
            res.v[2] = a*q0.v[2] + b*q1.v[2];

            normalize(res);
            return res;

        }
        else 
        {
            float a = t_;
            float b = t;
            res.w = a*q0.w + b*q1.w;
            res.v[0] = a*q0.v[0] + b*q1.v[0];
            res.v[1] = a*q0.v[1] + b*q1.v[1];
            res.v[2] = a*q0.v[2] + b*q1.v[2];

            normalize(res);
            return res;
        }
    }
    else
    {   
        float a = sin((0.5-t)*M_PI);
        float b = sin(t*M_PI);
        res.w = a*q0.w + b*q1.w;
        res.v[0] = a*q0.v[0] + b*q1.v[0];
        res.v[1] = a*q0.v[1] + b*q1.v[1];
        res.v[2] = a*q0.v[2] + b*q1.v[2];

        normalize(res);
        return res;

    }
    
}
Quaternion slerp2(Quaternion q0, Quaternion q1, const float t)
{
    Quaternion a = log(qproduct(qinverse(q0),q1));
    a.v[0] *= t;
    a.v[1] *= t;
    a.v[2] *= t;

    Quaternion b = qproduct(q0, exp(a));
    normalize(b);
    return b;

}

static std::string inputfile = "./my_surface.txt";
static bool Bspline = false;
static bool CRspline = false;
static int section_num;
static int ctrl_pts_num;

static Quaternion lastorient, currorient, rotate;
static float lastpoint[3] = {0.0,0.0,0.0}; 
static float currpoint[3]={0.0,0.0,0.0}; 
static float angle, c_norm;
static std::vector<std::vector<Point>> cross_sections;
static std::vector<std::vector<Point>> Vertexes;
static std::vector<std::vector<Point>> Original;
static std::vector<std::vector<Point>> Normals;
static std::vector<float> Scaling_factor;
static std::vector<Point> Translate_factor;
static std::vector<Quaternion> Rotate_factor;


void Bezier_discretize(const Point p0,const Point p1,const Point p2,const Point p3, std::vector<Point>& curve ,const int resolution)
{
    for (int i = 0; i < resolution; i++)
    {
        float t = 1.0/(resolution-1) * i;
        float B0 = pow(1.0 - t, 3); 
        float B1 = 3.0*t*pow(1.0-t,2); 
        float B2 = 3.0*pow(t,2)*(1.0-t);
        float B3 = pow(t,3);

        Point temp;
        temp.x = B0*p0.x + B1*p1.x + B2*p2.x + B3*p3.x;
        temp.y = B0*p0.y + B1*p1.y + B2*p2.y + B3*p3.y;
        temp.z = B0*p0.z + B1*p1.z + B2*p2.z + B3*p3.z;

        curve.push_back(temp);
    }    
    

}
void cubicBezier(const int ptsnum, const std::vector<Point> ctrlpts, std::vector<Point>& curve, const int resolution)
{
    for (int i = 0; i < ptsnum -3; i += 3)
    {
        Bezier_discretize(ctrlpts[i], ctrlpts[i+1],ctrlpts[i+2],ctrlpts[i+3],curve, resolution);
        
    }
}

void quat_discretize(const Quaternion q0, const Quaternion q1, const Quaternion q2, const Quaternion q3, std::vector<Quaternion>& curve, const int resolution)
{
    for (int i = 0; i < resolution; i++)
    {
        float t = 1.0/(resolution-1) * i;
        Quaternion q01 = slerp(q0, q1, t);
        Quaternion q12 = slerp(q1, q2, t);
        Quaternion q23 = slerp(q2, q3, t);
        Quaternion q02 = slerp(q01, q12, t);
        Quaternion q13 = slerp(q12, q23, t);
        Quaternion q03 = slerp(q02, q13, t);

        curve.push_back(q03);
    }
}
void quat_Bezier(const int ptsnum, const std::vector<Quaternion> ctrlpts, std::vector<Quaternion>& curve, const int resolution)
{
    for (int i = 0; i < ptsnum-3; i += 3)
    {
        quat_discretize(ctrlpts[i], ctrlpts[i+1],ctrlpts[i+2],ctrlpts[i+3], curve, resolution);
    }
}

void Bspline_discretize(const Point p0,const Point p1,const Point p2,const Point p3, std::vector<Point>& curve , const int resolution)
{
    for (int i = 0; i < resolution; i++)
    {
        float t = 1.0/(resolution-1) * i;
        float B0 = pow(1.0 - t, 3); B0 /= 6.0;
        float B1 = 3.0*pow(t,3) - 6.0*pow(t,2) + 4.0; B1 /= 6.0;
        float B2 = -3.0*pow(t,3) + 3.0*pow(t,2) + 3.0*t + 1; B2 /= 6.0;
        float B3 = pow(t,3); B3 /= 6.0;

        Point temp;
        temp.x = B0*p0.x + B1*p1.x + B2*p2.x + B3*p3.x;
        temp.y = B0*p0.y + B1*p1.y + B2*p2.y + B3*p3.y;
        temp.z = B0*p0.z + B1*p1.z + B2*p2.z + B3*p3.z;

        curve.push_back(temp);
    }
    
}
void cubicBspline(const int ptsnum,const std::vector<Point> ctrlpts, std::vector<Point>& curve, const int resolution)
{
    for (int i = 0; i < ptsnum-3; i++)
    {
        Bspline_discretize(ctrlpts[i], ctrlpts[i+1],ctrlpts[i+2],ctrlpts[i+3],curve, resolution);
    }
    Bspline_discretize(ctrlpts[ptsnum-3], ctrlpts[ptsnum-2], ctrlpts[ptsnum-1],ctrlpts[0], curve, resolution);
    Bspline_discretize(ctrlpts[ptsnum-2], ctrlpts[ptsnum-1], ctrlpts[0],ctrlpts[1], curve, resolution);
    Bspline_discretize(ctrlpts[ptsnum-1], ctrlpts[0], ctrlpts[1],ctrlpts[2], curve, resolution);
}

void CatmullRom(const int ptsnum,const std::vector<Point> ctrlpts, std::vector<Point>& new_ctrlpts, bool closed)
{
    if (!closed)
    {   
        Point temp;
        for (int i = 0; i < 3*ptsnum-2; i++)
        {
            new_ctrlpts.push_back(temp);
        }
    }
    else
    {
        Point temp;
        for (int i = 0; i < 3*ptsnum; i++)
        {
            new_ctrlpts.push_back(temp);
        }

    }

    for (int i = 0; i < ptsnum; i++)
    {
        Point temp{ctrlpts[i].x,ctrlpts[i].y,ctrlpts[i].z};
        new_ctrlpts[3*i] = temp;   
    }

    for (int i = 0; i < ptsnum-1; i++)
    {
        if (i>0)
        {
            new_ctrlpts[3*i+1].x = ctrlpts[i].x + (ctrlpts[i+1].x - ctrlpts[i-1].x)/6.0;
            new_ctrlpts[3*i+1].y = ctrlpts[i].y + (ctrlpts[i+1].y - ctrlpts[i-1].y)/6.0;
            new_ctrlpts[3*i+1].z = ctrlpts[i].z + (ctrlpts[i+1].z - ctrlpts[i-1].z)/6.0;
        }
        else
        {
            if (closed)
            {
                new_ctrlpts[1].x = ctrlpts[0].x + (ctrlpts[1].x - ctrlpts[ptsnum-1].x)/6.0;
                new_ctrlpts[1].y = ctrlpts[0].y + (ctrlpts[1].y - ctrlpts[ptsnum-1].y)/6.0;
                new_ctrlpts[1].z = ctrlpts[0].z + (ctrlpts[1].z - ctrlpts[ptsnum-1].z)/6.0;
            }
            else
            {
                new_ctrlpts[1] = ctrlpts[0];
            }
        }

        if (i<ptsnum-2)
        {
            new_ctrlpts[3*i+2].x = ctrlpts[i].x - (ctrlpts[i+2].x - ctrlpts[i].x)/6.0;
            new_ctrlpts[3*i+2].y = ctrlpts[i].y - (ctrlpts[i+2].y - ctrlpts[i].y)/6.0;
            new_ctrlpts[3*i+2].z = ctrlpts[i].z - (ctrlpts[i+2].z - ctrlpts[i].z)/6.0;
        }
        else
        {
            if (closed)
            {
            new_ctrlpts[3*i+2].x = ctrlpts[ptsnum-1].x + (ctrlpts[ptsnum-2].x - ctrlpts[0].x)/6.0;
            new_ctrlpts[3*i+2].y = ctrlpts[ptsnum-1].y + (ctrlpts[ptsnum-2].y - ctrlpts[0].y)/6.0;
            new_ctrlpts[3*i+2].z = ctrlpts[ptsnum-1].z + (ctrlpts[ptsnum-2].z - ctrlpts[0].z)/6.0;

            new_ctrlpts[3*i+4].x = ctrlpts[ptsnum-1].x - (ctrlpts[ptsnum-2].x - ctrlpts[0].x)/6.0;
            new_ctrlpts[3*i+4].y = ctrlpts[ptsnum-1].y - (ctrlpts[ptsnum-2].y - ctrlpts[0].y)/6.0;
            new_ctrlpts[3*i+4].z = ctrlpts[ptsnum-1].z - (ctrlpts[ptsnum-2].z - ctrlpts[0].z)/6.0;

            new_ctrlpts[3*i+5].x = ctrlpts[0].x - (ctrlpts[1].x - ctrlpts[ptsnum-1].x)/6.0;
            new_ctrlpts[3*i+5].y = ctrlpts[0].y - (ctrlpts[1].y - ctrlpts[ptsnum-1].y)/6.0;
            new_ctrlpts[3*i+5].z = ctrlpts[0].z - (ctrlpts[1].z - ctrlpts[ptsnum-1].z)/6.0;
            }
            else
            {
                new_ctrlpts[3*ptsnum-4] = ctrlpts[ptsnum-1];
            }
        }

    }

}
void quat_CR(const int ptsnum, const std::vector<Quaternion> ctrlpts, std::vector<Quaternion>& new_ctrlpts)
{
    if (ptsnum < 3) return;

    Quaternion temp{0,{0,0,0}};
    for (int i = 0; i < ptsnum*3-2; i++)
    {
        new_ctrlpts.push_back(temp);
    }

    for (int i = 0; i < ptsnum; i++)
    {
        new_ctrlpts[3*i] = ctrlpts[i];
    }

    for (int i = 0; i < ptsnum-1; i++)
    {
        Quaternion qi = ctrlpts[i];
        Quaternion qf = ctrlpts[i+1]; 

        if (i>0)

        {   
            Quaternion qb = ctrlpts[i-1];
            Quaternion a = qproduct(qinverse(qb),qf);
            Quaternion b{log(a).w/6,{log(a).v[0]/6,log(a).v[1]/6,log(a).v[2]/6}};
            Quaternion res = qproduct(qi , exp(b));
            normalize(res);
            
            new_ctrlpts[3*i+1] = res;
        }
        else
        {
            new_ctrlpts[1] = ctrlpts[0];
        }

        if (i< ptsnum-2)
        {
            Quaternion qff = ctrlpts[i+2];
            Quaternion a = qproduct(qinverse(qi),qff);
            Quaternion b{log(a).w/6,{log(a).v[0]/6,log(a).v[1]/6,log(a).v[2]/6}};
            Quaternion c = qproduct(exp(b), qinverse(qf));
            normalize(c);
            Quaternion res = qinverse(c);

            new_ctrlpts[3*i+2] = res;

        }
        else
        {
            new_ctrlpts[3*ptsnum-4] = ctrlpts[ptsnum-1];
        }
    }

}

void Scalespline(const int section_num,const std::vector<float> Scale_factor, std::vector<float>& Scale_spline, const int resolution)
{
    std::vector<Point> scale;
    std::vector<Point> new_ctrlpts;
    std::vector<Point> res;

    //float -> Point
    if (section_num != Scale_factor.size()) { cout << section_num << " " << Scale_factor.size() << endl;}
    
    for (int i = 0; i < Scale_factor.size(); i++)
    {
        Point temp{Scale_factor[i],0,0};
        scale.push_back(temp);
    }


    CatmullRom(section_num, scale, new_ctrlpts, false);
    cubicBezier(new_ctrlpts.size(), new_ctrlpts, res, resolution);

    for (int i = 0; i < res.size(); i++)
    {
        Scale_spline.push_back(res[i].x);
    }
    
}
void Translatespline(const int section_num, const std::vector<Point> Translate_factor, std::vector<Point>& Translate_spline, const int resolution)
{
    std::vector<Point> new_ctrlpts;
    if (section_num != Translate_factor.size()) { cout << section_num << " " << Translate_factor.size() << endl;}

    CatmullRom(Translate_factor.size(), Translate_factor, new_ctrlpts, false);
    cubicBezier(new_ctrlpts.size(), new_ctrlpts, Translate_spline, resolution);
}
void Rotatespline(const int section_num, const std::vector<Quaternion> Rotate_factor, std::vector<Quaternion>& Rotate_spline,const int resolution)
{
    ///Rotate_factor(angle, axis), Rotate_spline(cos(angle), sin(angle)*v)///
    std::vector<Quaternion> ctrlpts;
    std::vector<Quaternion> new_ctrlpts;

    for (int i = 0; i < Rotate_factor.size(); i++)
    {
        float angle = Rotate_factor[i].w/2;
        float x = Rotate_factor[i].v[0];
        float y = Rotate_factor[i].v[1];
        float z = Rotate_factor[i].v[2];
        float length = sqrt(x*x + y*y + z*z);
        float sine = sin(angle);
        if (length < 0.0001 || angle < 0.0001) {
            Quaternion res{1, {0, 0, 0}};
            ctrlpts.push_back(res);
        }
        else
        {
            Quaternion res{cos(angle), {sine*x, sine*y, sine*z}};
            ctrlpts.push_back(res);
        }
        
    }
    
    quat_CR(Rotate_factor.size(), ctrlpts, new_ctrlpts);
    quat_Bezier(new_ctrlpts.size(), new_ctrlpts, Rotate_spline, resolution);

}

void setVertex(const int section_num, std::vector<std::vector<Point>>& Vertexes, const int resolution)
{
    std::vector<std::vector<Point>> new_sections;
    std::vector<float> scale_spline;
    std::vector<Point> trans_spline;
    std::vector<Quaternion> rot_spline;

    Scalespline(section_num, Scaling_factor, scale_spline, resolution);
    Translatespline(section_num, Translate_factor, trans_spline, resolution);
    Rotatespline(section_num, Rotate_factor, rot_spline, resolution);

    if (scale_spline.size() != trans_spline.size())
    {
        printf("scale and translate diff");
        return;
    }
    else
    {
        if (scale_spline.size() != rot_spline.size())
        {
            printf("rot num diff");
            return;
        }
    }

    for (int i = 0; i < scale_spline.size(); i++)
    {
        std::vector<Point> tempsection;

        float s = scale_spline[i];
        Quaternion q = rot_spline[i];
        Point t = trans_spline[i];

        for (int j = 0; j < ctrl_pts_num; j++)
        {
            Quaternion scaled{0,{0,0,0}};
            scaled.v[0] = cross_sections[0][j].x*s;
            scaled.v[1] = cross_sections[0][j].y*s;
            scaled.v[2] = cross_sections[0][j].z*s;

            Quaternion rotated = qrotate(scaled, q);

            Point translated{rotated.v[0] + t.x,rotated.v[1] + t.y,rotated.v[2] + t.z};

            tempsection.push_back(translated); 
            ///how to handle for different input ctrl point?
        }
        new_sections.push_back(tempsection);
    }

    if (Bspline == true)
    {
        for (int i = 0; i < new_sections.size(); i++)
        {
            Point temp;
            std::vector<Point> section_verts;
            cubicBspline(new_sections[i].size(), new_sections[i], section_verts,resolution);
            Vertexes.push_back(section_verts);
        }
    }
    else if (CRspline == true)
    {
        for (int i = 0; i < new_sections.size(); i++)
        {
            Point temp;
            std::vector<Point> section_verts;
            std::vector<Point> new_ctrlpts;   

            CatmullRom(new_sections[i].size(), new_sections[i], new_ctrlpts,resolution);
            cubicBezier(new_ctrlpts.size(), new_ctrlpts, section_verts, resolution);
       

            Vertexes.push_back(section_verts);
        }
    }    

}

void original_sections(const int section_num, std::vector<std::vector<Point>>& Original, const int resolution)
{

    for (int i = 0; i < cross_sections.size(); i++)
    {
        vector<Point> single_sec;
        float s = Scaling_factor[i];
        
        Point t = Translate_factor[i];
        float sine = sin(Rotate_factor[i].w/2);
        Quaternion q{cos(Rotate_factor[i].w/2),{sine*Rotate_factor[i].v[0],sine*Rotate_factor[i].v[1],sine*Rotate_factor[i].v[2]}};
        for (int j = 0; j < cross_sections[i].size(); j++)
        {
            float x = cross_sections[i][j].x;
            float y = cross_sections[i][j].y;
            float z = cross_sections[i][j].z;

            Quaternion scaled{0, {x*s,y*s,z*s}};
            Quaternion rotated = qrotate(scaled,q);
            Point temp{rotated.v[0]+t.x,rotated.v[1]+t.y,rotated.v[2]+t.z};

            single_sec.push_back(temp);
        }
        
        Original.push_back(single_sec);
    }
}

void setNormal(const vector<vector<Point>>& Vertexes, vector<vector<Point>>& Normals)
{
    int a = Vertexes.size();
    int b = Vertexes[0].size();

    for (int i = 0; i < a; i++)
    {
        Point temp = {0,0,0};
        vector<Point> tem;
        for (int j = 0; j < b; j++)
        {
            tem.push_back(temp);
        }
        Normals.push_back(tem);
    }

    vector<vector<Point>> totalsecnorm;
    for (int i = 0; i < a-1; i++) 
    {
        vector<Point> onesecnorm;
        for (int j = 0; j < b; j++)
        {
            float v1[3] = {Vertexes[i+1][j].x-Vertexes[i][j].x,Vertexes[i+1][j].y-Vertexes[i][j].y,Vertexes[i+1][j].z-Vertexes[i][j].z};
            float v2[3] = {Vertexes[i+1][j].x-Vertexes[i][j+1].x,Vertexes[i+1][j].y-Vertexes[i][j+1].y,Vertexes[i+1][j].z-Vertexes[i][j+1].z};
            float cross[3];
            crossproduct(v1, v2, cross);
            normalize(cross);
            Point temp{cross[0],cross[1],cross[2]};
            onesecnorm.push_back(temp);
        }
        totalsecnorm.push_back(onesecnorm);
    }
    for (int j = 1; j < b; j++)
    {
        for (int i = 0; i < a; i++)
        {
            if (i == 0)
            {
                Normals[i][j].x = (totalsecnorm[i][j-1].x + totalsecnorm[i][j].x)/2;
                Normals[i][j].y = (totalsecnorm[i][j-1].y + totalsecnorm[i][j].y)/2;
                Normals[i][j].z = (totalsecnorm[i][j-1].z + totalsecnorm[i][j].z)/2;

            }
            else if(i == a-1)
            {
                Normals[i][j].x = (totalsecnorm[i-1][j-1].x + totalsecnorm[i-1][j].x)/2;
                Normals[i][j].y = (totalsecnorm[i-1][j-1].y + totalsecnorm[i-1][j].y)/2;
                Normals[i][j].z = (totalsecnorm[i-1][j-1].z + totalsecnorm[i-1][j].z)/2;           
            }
            else
            {
                Normals[i][j].x = (totalsecnorm[i][j-1].x + totalsecnorm[i][j].x + totalsecnorm[i-1][j-1].x + totalsecnorm[i-1][j].x )/4;
                Normals[i][j].y = (totalsecnorm[i][j-1].y + totalsecnorm[i][j].y + totalsecnorm[i-1][j-1].y + totalsecnorm[i-1][j].y )/4;
                Normals[i][j].z = (totalsecnorm[i][j-1].z + totalsecnorm[i][j].z + totalsecnorm[i-1][j-1].z + totalsecnorm[i-1][j].z )/4;           
            }
        }
    }

    for (int i = 0; i < a; i++)
    {
        if (i == 0)
        {
            Normals[i][0].x  = (totalsecnorm[i][b-1].x  + totalsecnorm[i][0].x )/2;
            Normals[i][0].y = (totalsecnorm[i][b-1].y + totalsecnorm[i][0].y)/2;
            Normals[i][0].z = (totalsecnorm[i][b-1].z + totalsecnorm[i][0].z)/2;
        }
        else if (i == a-1)
        {
            Normals[i][0].x  = (totalsecnorm[i-1][b-1].x  + totalsecnorm[i-1][0].x )/2;
            Normals[i][0].y  = (totalsecnorm[i-1][b-1].y  + totalsecnorm[i-1][0].y )/2;
            Normals[i][0].z = (totalsecnorm[i-1][b-1].z + totalsecnorm[i-1][0].z)/2;
        }
        else
        {
            Normals[i][0].x  = (totalsecnorm[i][b-1].x  + totalsecnorm[i][0].x  + totalsecnorm[i-1][b-1].x  + totalsecnorm[i-1][0].x  )/4;
            Normals[i][0].y = (totalsecnorm[i][b-1].y + totalsecnorm[i][0].y + totalsecnorm[i-1][b-1].y + totalsecnorm[i-1][0].y )/4;
            Normals[i][0].z = (totalsecnorm[i][b-1].z + totalsecnorm[i][0].z + totalsecnorm[i-1][b-1].z + totalsecnorm[i-1][0].z )/4;           

        }
    }


}

void drawSweptSurface()
{   
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // for (int i = 0 ; i < Original.size(); i++)
    // {
    //     glBegin(GL_LINE_LOOP);
    //     glColor3f(1,0,0);
    //     for (int j = 0; j < Original[i].size(); j++)
    //     {
    //         glVertex3f(Original[i][j].x,Original[i][j].y,Original[i][j].z);
    //     }
    //     glEnd();
    // }
    
    GLfloat silver_amb[] = { 0.19225, 0.19225, 0.19225, 1.0};
    GLfloat silver_diff[] = { 0.50754, 0.50754, 0.50754, 1.0};
    GLfloat silver_spec[] = { 0.508273, 0.508273, 0.508273, 1.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT, silver_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, silver_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, silver_spec);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.4*128);


    glPointSize(1.0);

    glBegin(GL_QUAD_STRIP);
    glColor3f(1,1,1);
    for (int i = 0 ; i < Vertexes.size()-1; i++)
    {
        for (int j = 0; j < Vertexes[i].size(); j++)
        {
            glNormal3f(-Normals[i][j].x,-Normals[i][j].y,-Normals[i][j].z);
            glVertex3f(Vertexes[i][j].x,Vertexes[i][j].y,Vertexes[i][j].z);
            glNormal3f(-Normals[i+1][j].x,-Normals[i+1][j].y,-Normals[i+1][j].z);
            glVertex3f(Vertexes[i+1][j].x,Vertexes[i+1][j].y,Vertexes[i+1][j].z);
        }
    }
    glEnd();
    
}

void drawrubberball()
{
    glPushMatrix();
    GLfloat rubber_amb[] = { 0.0, 0.05, 0.0, 1.0};
    GLfloat rubber_diff[] = { 0.4, 0.5, 0.4, 1.0};
    GLfloat rubber_spec[] = { 0.04, 0.7, 0.04, 1.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT, rubber_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, rubber_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, rubber_spec);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.078125*128);

    glutSolidSphere(10.0,100,100);
    glPopMatrix();
    
}

void drawcube()
{
    glPushMatrix();
    GLfloat cube_amb[] = { 0.1745, 0.01175, 0.01175, 1.0};
    GLfloat cube_diff[] = { 0.61424, 0.04136, 0.04136, 1.0};
    GLfloat cube_spec[] = { 0.727811, 0.626959, 0.626959, 1.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT, cube_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, cube_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, cube_spec);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.6*128.0);

    glutSolidCube(10);
    glPopMatrix();
}

static vector<float> points;
void impsurface()
{
    const float PI = 3.1415926;

    for (float y = -3; y < 4.9; y += 0.1)
    {
        
        GLfloat coeff = sqrt(pow(log(y+3.2),2)+0.02);  

        for (float theta = 0; theta < 2*PI; theta += 2*PI/100)
        {   
            GLfloat x = coeff*cos(theta);
            GLfloat z = coeff*sin(theta);
            

            points.push_back(x);
            points.push_back(y);
            points.push_back(z);
            
        }
    }
    cout << points.size() << endl;
}

void drawimpsurf()
{   
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    GLfloat gold_amb[] = { 0.24725, 0.1995, 0.0745, 1.0};
    GLfloat gold_diff[] = { 0.75164, 0.60648, 0.22648, 1.0};
    GLfloat gold_spec[] = { 0.628281, 0.555802, 0.366065, 1.0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, gold_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, gold_diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gold_spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.4*128);

    glBegin(GL_QUAD_STRIP);
    for (int y = 0; y < 79; y++)
    {
        for (int i = 0; i < 100; i++)
        {
            glVertex3f(points[300*y + 3*i],points[300*y + 3*i+1],points[300*y + 3*i+2]);
            glVertex3f(points[300*(y+1) + 3*i],points[300*(y+1) + 3*i+1],points[300*(y+1) + 3*i+2]);

        }
        glVertex3f(points[300*y ],points[300*y + 1],points[300*y + 2]);
        glVertex3f(points[300*(y+1)],points[300*(y+1) + 1],points[300*(y+1) + 2]);

    }

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    glPushMatrix();
    // drawSweptSurface();
    // glTranslatef(-30,0,0);
    // drawemcube();
    // glTranslatef(0.0,0.0,-30.0);
    // drawrubberball();
    drawimpsurf();
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}


void Lighting()
{
    GLfloat light_ambient0[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat light_diffuse0[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular0[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position0[] = { 20.0, 20.0, 20.0, 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

    glEnable(GL_LIGHT0);


    GLfloat light_ambient1[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position1[] = { -40.0, 40.0, 20.0, 1.0 };
    GLfloat spot_direction[] = { 2.0, -2.0, -1.0 };

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

    //glEnable(GL_LIGHT1);
    

    GLfloat light_ambient2[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse2[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular2[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position2[] = { 20.0, 20.0, 20.0, 0.0 };

    glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position2);

    //glEnable(GL_LIGHT2);

    GLfloat light_ambient3[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse3[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular3[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position3[] = { 20.0, 20.0, 20.0, 0.0 };

    glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient3);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse3);
    glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular3);
    glLightfv(GL_LIGHT3, GL_POSITION, light_position3);

    //glEnable(GL_LIGHT3);


    GLfloat light_ambient4[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse4[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular4[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position4[] = { 20.0, 20.0, 20.0, 0.0 };

    glLightfv(GL_LIGHT4, GL_AMBIENT, light_ambient4);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse4);
    glLightfv(GL_LIGHT4, GL_SPECULAR, light_specular4);
    glLightfv(GL_LIGHT4, GL_POSITION, light_position4);

    //glEnable(GL_LIGHT4);


}

void reshape(int w, int h)
{
    width = glutGet(GLUT_WINDOW_WIDTH);
    height = glutGet(GLUT_WINDOW_HEIGHT);
    float aspectRatio = (float)w/(float)h;
        
    float x0 = cam_dist*cam_x;
	float y0 = cam_dist*cam_y;
	float z0 = cam_dist*cam_z; 

    glViewport(0, 0, w, h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
    Lighting();
	gluPerspective( fov /*field of view angle*/,
					aspectRatio,
					0.1 /*near clipping plane*/,
					1000.0 /* far clipping plane */ );
   
	glMatrixMode( GL_MODELVIEW );
    glLoadIdentity(); 
	gluLookAt( x0,y0,z0, 
               lookat_x,lookat_y,lookat_z, 
                vx,vy,vz );

}
void keyboardCB(unsigned char keyPressed, int x, int y)
{
    switch (keyPressed) {
    case 'f':
        if (fullScreen == true) {
            glutReshapeWindow(width,height);
            fullScreen = false;
        } else {
            glutFullScreen();
            fullScreen = true;
        }
        break;
    case 'q':
        exit(0);
        break;
    case 'r':

        cam_dist = 200;
        cam_x= 0; cam_y = 0; cam_z = 1;
        lookat_x = 0; lookat_y = 0;
        fov = 45;
        vx = 0; vy = 1; vz = 0;
        reshape(width, height);
        break;
    case 'p':
        glEnable(GL_LIGHTING);
        reshape(width, height);
        break;
    case 'o':
        glDisable(GL_LIGHTING);
        reshape(width, height);
    }
    glutPostRedisplay();
}

void mouseCB(int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        mouseRotatePressed = false;
        mouseMovePressed   = false;
        mouseZoomPressed   = false;
        mouseDollyPressed  = false;
        mouseSeekPressed   = false;

    } else {
        if (button==GLUT_LEFT_BUTTON && GLUT_ACTIVE_SHIFT==glutGetModifiers())
        {
			// do something here
			//std::cout << "translate click" << std::endl;
            lastx = x;
            lasty = y;
            lastlookat_x = lookat_x;
            lastlookat_y = lookat_y;


            mouseRotatePressed = false;
            mouseMovePressed   = true;
            mouseZoomPressed   = false;
            mouseDollyPressed  = false;
            mouseSeekPressed   = false;
                   
        }
        else if (button==GLUT_LEFT_BUTTON && GLUT_ACTIVE_CTRL==glutGetModifiers())
        {
			// do something here
			//std::cout << "zoom click" << std::endl;
            lastx = x;
            lasty = y;
            lastfov = fov;

            mouseRotatePressed = false;
            mouseMovePressed   = false;
            mouseZoomPressed   = true;
            mouseDollyPressed  = false;
            mouseSeekPressed   = false;
        
        }
        else if (button==GLUT_RIGHT_BUTTON && GLUT_ACTIVE_CTRL==glutGetModifiers())
        {
			// do something here
			//std::cout << "dolly click" << std::endl;
            lastx = x;
            lasty = y;
            
            lastcam_dist = cam_dist;

            mouseRotatePressed = false;
            mouseMovePressed   = false;
            mouseZoomPressed   = false;
            mouseDollyPressed  = true;
            mouseSeekPressed   = false;
        
        }
        
        else if (button==GLUT_LEFT_BUTTON)
        {   
            float len;

            //projection
            lastpoint[0] = (2.0*x - width)/width;
            lastpoint[1] = (height - 2.0*y)/height;
            len = lastpoint[0]*lastpoint[0] + lastpoint[1]*lastpoint[1];
			if (len > 1)
            {
                lastpoint[0] /= sqrt(len); lastpoint[1] /= sqrt(len);
                lastpoint[2] = 0;
            }
            else 
            { 
                lastpoint[2] = sqrt(1.0 - lastpoint[0]*lastpoint[0] - lastpoint[1]*lastpoint[1]); 
            }

            lastorient.w = 0;
            lastorient.v[0] = cam_x; lastorient.v[1] = cam_y; lastorient.v[2] =cam_z;
            normalize(lastorient);
            
            lastvx = vx; lastvy = vy; lastvz = vz;

            mouseMovePressed   = false;
            mouseRotatePressed = true;
            mouseZoomPressed   = false;
            mouseDollyPressed  = false;
            mouseSeekPressed   = false;
        
        }
    }
    glutPostRedisplay();
}

void motionCB(int x, int y) {
    if (mouseRotatePressed == true)
	{
        float len;
        float axis[3];

        currpoint[0] = (2.0*x - width)/width;
        currpoint[1] = (height - 2.0*y)/height;
        len = currpoint[0]*currpoint[0] + currpoint[1]*currpoint[1];
		if (len> 1)
        {
            currpoint[0] /= sqrt(len); currpoint[1] /= sqrt(len);
            currpoint[2] = 0;
        }
        else { currpoint[2] = sqrt(1.0 - currpoint[0]*currpoint[0] - currpoint[1]*currpoint[1]); }
       
        crossproduct(currpoint,lastpoint,axis);   
        len = sqrt((pow(axis[0],2))+(pow(axis[1],2))+(pow(axis[2],2)));      
        angle = atan2(len, innerproduct(lastpoint,currpoint));      
        normalize(axis); 



        rotate.w = cos(angle/2);
        rotate.v[0] = axis[0]*sin(angle/2);
        rotate.v[1] = axis[1]*sin(angle/2); 
        rotate.v[2] = axis[2]*sin(angle/2); 

        currorient = qrotate(lastorient,rotate);


        normalize(currorient);

        
        cam_x = currorient.v[0]; cam_y = currorient.v[1]; cam_z = currorient.v[2];
        
        
        float look[3] = {cam_x,cam_y,cam_z};
        float worldUp[3] = {0.0,1.0,0.0};
        float right[3], up[3];
        crossproduct(worldUp,look,right);
        crossproduct(look, right,up);
        normalize(up);
        vx = up[0]; vy = up[1]; vz = up[2];

        // float normal[3];
        // float upvec[3] = {vx,vy,vz};
        // crossproduct(currorient.v,axis,normal);
        // normalize(normal);
        // float a = innerproduct(axis, upvec);
        // float b = sqrt(1-a*a);
        // upvec[0] = a*axis[0] + b*normal[0];
        // upvec[1] = a*axis[1] + b*normal[1];
        // upvec[2] = a*axis[2] + b*normal[2];
        // normalize(upvec);
        // vx = upvec[0]; vy = upvec[1]; vz = upvec[2];




		reshape(width, height);
	}
    else if (mouseMovePressed == true)
    {

    
        lookat_x = lastlookat_x + (float)(lastx-x)*0.005;       
        lookat_y = lastlookat_y + (float)(y-lasty)*0.005;

        reshape(width, height);

    }
    else if (mouseZoomPressed == true)
    {

        fov = lastfov + (float)(y - lasty)*0.01;
		reshape(width, height);

    }
    else if (mouseDollyPressed == true)
    {

        cam_dist = lastcam_dist + (float)(y - lasty)*0.01;
		reshape(width, height);

    }

    glutPostRedisplay();
}

static vector<GLfloat> obj1_v;
static vector<GLfloat> obj1_vn;
static vector<GLfloat> obj1_f;

void loadobj(string filename, vector<GLfloat> obj_v, vector<GLfloat> obj_vn, vector<GLfloat> obj_f)
{   
    string line;
    ifstream file(filename);
    string str;

    if (!file) return;
    while (getline(file, line))
    {   
        
        if(line.substr(0,2) == "v ")
        {
            
            float x, y, z;
            istringstream(str) >> x >> y >> z;
            obj_v.push_back(x);
            obj_v.push_back(y);
            obj_v.push_back(z);
        }
        else if (line.substr(0,2) == "vn")
        {
            
            float x, y, z;
            istringstream(str) >> x >> y >> z;
            obj_vn.push_back(x);
            obj_vn.push_back(y);
            obj_vn.push_back(z);

         }
        else if (line.substr(0,2) == "f ")
        {
            cout<< "hif" << endl;
            float x, y, z;
            istringstream(str) >> x >> y >> z;
            cout << x << y << z<< endl;

         }



    }
    

}
void readFile()
{
    string line, curve;
    vector<Point> single_section;
    Point temp;
    Quaternion rtemp;
    float value;
    
    std::ifstream file(inputfile);
    if (file.is_open())
    {
        std::getline(file, line);
        istringstream ss(line);
        ss >> curve;
        ss.clear();
        ss.str("");

        std::cout << curve << endl;
        curve == "BSPLINE"? (Bspline = true) : (CRspline = true);

        std::getline(file, line);
        ss.str(line);
        ss >> section_num;
        ss.clear();
        ss.str("");

        std::cout << section_num << endl;

        std::getline(file,line);
        ss.str(line);
        ss >> ctrl_pts_num;
        ss.clear();
        ss.str("");

        std::cout << ctrl_pts_num << endl;
        while (getline(file, line))
        {   
            if(line[0] == '#' || line.length() <= 1) continue;

            for (int i = 0; i < ctrl_pts_num; i++)
            {   
                
                ss.str(line);
                ss >> temp.x >> temp.z;
                temp.y = 0;
                single_section.push_back(temp);

                ss.clear();
                ss.str("");
                getline(file, line);
            }
            cross_sections.push_back(single_section);

            ss.str(line);
            ss >> value;
            Scaling_factor.push_back(value);
            ss.clear();
            ss.str("");
            
            getline(file, line);
            ss.str(line);
            ss >> rtemp.w >> rtemp.v[0] >> rtemp.v[1] >> rtemp.v[2];
            Rotate_factor.push_back(rtemp);
            ss.clear();
            ss.str("");

            getline(file, line);
            ss.str(line);
            ss >> temp.x >> temp.y >> temp.z;
            Translate_factor.push_back(temp);
            ss.clear();
            ss.str("");
        }
    }
    file.close();
}

void idle() { glutPostRedisplay(); }

int main(int argc, char** argv) {

    readFile();
    loadobj("./Rose.obj", obj1_v, obj1_vn, obj1_f);
    setVertex(section_num, Vertexes, 20);
    setNormal(Vertexes, Normals);
    //original_sections(section_num, Original, 20);
    impsurface();



    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutCreateWindow("3D viewer");
    reshape(width, height);
    


    glClearColor(0.0, 0.0, 0.0, 1.0);
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_LIGHTING);

    

    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardCB);
    glutReshapeFunc(reshape);
    glutMotionFunc(motionCB);
    glutMouseFunc(mouseCB);
    glutMainLoop();

    glDisableClientState(GL_VERTEX_ARRAY);

    return 0;
}
