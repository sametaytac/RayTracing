#include <iostream>
#include "parseScene.h"
#include "writePPM.h"
#include "Camera.h"
#include "Material.h"
#include "Vertex.h"
#include "ImagePlane.h"
#include "Backcolor.h"
#include "Triangle.h"
#include "Sphere.h"
#include "VectorCl.h"
#include "PointLight.h"
#include "AmbientLight.h"
#include <vector>
#include <math.h>
using namespace std;
extern vector <Vertex> vert;
extern vector<Material> mat;
extern vector<Triangle> tri;
extern vector<Sphere> sph;
extern vector<Camera> cam;
extern Backcolor color1;
extern AmbientLight alight;
extern vector<PointLight> polight;

VectorCl conv(float* a){
    VectorCl b;
    b.vec[0]=a[0];
    b.vec[1]=a[1];
    b.vec[2]=a[2];
    return b;


}



float det(float* a,float* b,float* c){
return a[0]*(b[1]*c[2]-b[2]*c[1])+a[1]*(c[0]*b[2]-b[0]*c[2])+a[2]*(b[0]*c[1]-b[1]*c[0]);
}

VectorCl carp(VectorCl a,float b){

    VectorCl c;
    c.vec[0]=a.vec[0]*b;
    c.vec[1]=a.vec[1]*b;
    c.vec[2]=a.vec[2]*b;
    return c;

}
VectorCl topla(VectorCl a,VectorCl b){

    VectorCl c;
    c.vec[0]=a.vec[0]+b.vec[0];
    c.vec[1]=a.vec[1]+b.vec[1];
    c.vec[2]=a.vec[2]+b.vec[2];
    return c;

}
VectorCl cross(VectorCl a,VectorCl b){
    VectorCl c;
    c.vec[0]=a.vec[1]*b.vec[2]-a.vec[2]*b.vec[1];
    c.vec[1]=a.vec[2]*b.vec[0]-a.vec[0]*b.vec[2];
    c.vec[2]=a.vec[0]*b.vec[1]-a.vec[1]*b.vec[0];


return c;



}

float dot(VectorCl a,VectorCl b){

    return a.vec[0]*b.vec[0]+a.vec[1]*b.vec[1]+a.vec[2]*b.vec[2];


}

int main(int argc, char* argv[])
{
    //
    // Test XML parsing
    //
    bool result = parseSceneXML(argv[1]);

for(int sy=0;sy<cam.size();sy++){
    VectorCl vecu;
    vecu=cross(conv(cam[sy].gaze),conv(cam[sy].up));
    float data[cam[sy].plane.nx*cam[sy].plane.ny*3];


        for(int j=0;j<(int)cam[sy].plane.ny;j++){
            float sv=cam[sy].plane.bottom+(cam[sy].plane.top-cam[sy].plane.bottom)*(j+0.5)/cam[sy].plane.ny;

    for(int i=0;i<(int)cam[sy].plane.nx;i++){
        float su=cam[sy].plane.left+(cam[sy].plane.right-cam[sy].plane.left)*(i+0.5)/cam[sy].plane.nx;
            VectorCl ray1=topla((topla(carp(vecu,su),carp(conv(cam[sy].up),sv))),carp(conv(cam[sy].gaze),cam[sy].plane.distance));



  //cout<<ray1.vec[0]<<","<<ray1.vec[1]<<","<<ray1.vec[2]<<endl;
               float t=10000000;

            //RAY SPHERE INTERSECTION

               float disc;
               for(int r=0;r<sph.size();r++){
                   int k;
                   for(k=0;k<vert.size();k++){
                       if(sph[r].centerid==vert[k].vertexid)
                           break;}



                       VectorCl oeksic=(topla(conv(cam[sy].position),carp(conv(vert[k].coor),-1)));
                       disc=pow((dot(ray1,oeksic)),2)-((dot(ray1,ray1))*((dot(oeksic,oeksic))-pow((sph[r].radius),2)));
                       //cout<<sph.size()<<endl;
                       if(disc>0)
                       {
                          float temp=min((-dot(ray1,oeksic)+sqrt(disc))/dot(ray1,ray1),(-dot(ray1,oeksic)-sqrt(disc))/dot(ray1,ray1));
                          if(temp<t)
                              t=temp;

                       }

                   }


/////////TRIANGLE INTERSECTION////////////////


for(int b=0;b<tri.size();b++){
vector <Vertex> vert1;
for(int y=0;y<3;y++){
int hu; 
for(hu=0;hu<vert.size();hu++){
                       if(tri[b].vertexlerid[y]==vert[hu].vertexid)
                           break;}
vert1.push_back(vert[hu]);
}
float ilk[3]={(vert1[0].coor[0]-vert1[1].coor[0]),vert1[0].coor[1]-vert1[1].coor[1],vert1[0].coor[2]-vert1[1].coor[2]};
float ikin[3]={  vert1[0].coor[0]-vert1[2].coor[0], vert1[0].coor[1]-vert1[2].coor[1],   vert1[0].coor[2]-vert1[2].coor[2]  };
float ae[3]={vert1[0].coor[0]-cam[sy].position[0], vert1[0].coor[1]-cam[sy].position[1] , vert1[0].coor[2]-cam[sy].position[2]};
float ra[3]={  ray1.vec[0],ray1.vec[1],ray1.vec[2] };
float a=det(ilk,ikin,ra);

float te=det(ilk,ikin,ae)/a;
if(te<t){

float teta=det(ilk,ae,ra)/a;
/*float yi[3]={1,1,1};
float yi2[3]={12,3,67};
float yi3[3]={2,3,4};
cout<<det(yi,yi2,yi3)<<endl;*/
if(teta<1 and teta>0)
{
float beta=det(ae,ikin,ra)/a;
if(beta>0 and teta+beta<1)
{t=te;}
}

}

}





int sa=j;
	if(j>(int)(cam[sy].plane.ny/2))
		 sa=j-2*(j-(cam[sy].plane.ny/2));
if(j<(int)(cam[sy].plane.ny/2))
		sa=j+2*((cam[sy].plane.ny/2)-j);
if(sa==cam[sy].plane.ny)
sa--;

    if(t<10000000){
        data[/*(cam[sy].plane.nx*cam[sy].plane.ny*3)-*/(i*3+cam[sy].plane.nx*sa*3)]=150;
        data[/*(cam[sy].plane.nx*cam[sy].plane.ny*3)-*/((i*3+cam[sy].plane.nx*sa*3)+1)]=150;
        data[/*(cam[sy].plane.nx*cam[sy].plane.ny*3)-*/((i*3+cam[sy].plane.nx*sa*3)+2)]=150;
    }
    else{
	if(sa==cam[sy].plane.ny/2+80)
	cout<<sa<<" ";
        data[/*(cam[sy].plane.nx*cam[sy].plane.ny*3)-*/(i*3+cam[sy].plane.nx*sa*3)]=color1.color[0];
        data[/*(cam[sy].plane.nx*cam[sy].plane.ny*3)-*/((i*3+cam[sy].plane.nx*sa*3)+1)]=color1.color[1];
        data[/*(cam[sy].plane.nx*cam[sy].plane.ny*3)-*/((i*3+cam[sy].plane.nx*sa*3)+2)]=color1.color[2];
    }

    }}



//for(int i=0;i<ray.size();i++)
  //  cout<<ray[i].vec[0]<<ray[i].vec[1]<<ray[i].vec[2]<<endl;
const char* ee;
ee=&cam[sy].filename[0];
    writePPM(ee, cam[sy].plane.nx, cam[sy].plane.ny, data);
}
    if (result)
    {
        std::cout << "Scene file parse successfully" << std::endl; 
    }
    else
    {
        std::cout << "ERROR parsing the scene file" << std::endl; 
    }
    //
    // Test PPM write
    //

   

    return 0;
}
