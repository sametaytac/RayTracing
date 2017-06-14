#include <iostream>
#include <cstdio>
#include "tinyXML/tinyxml.h"
#include "Camera.h"
#include "Material.h"
#include "Vertex.h"
#include "ImagePlane.h"
#include "Backcolor.h"
#include "Triangle.h"
#include "Sphere.h"
#include "PointLight.h"
#include "AmbientLight.h"
#include <string.h>
#include <vector>
using namespace std;
vector <Vertex> vert;
vector<Material> mat;
vector<Triangle> tri;
vector<Sphere> sph;
vector<Camera> cam;
Backcolor color1;
AmbientLight alight;
vector<PointLight> polight;
bool parseSceneXML(const char* filename)
{
	TiXmlDocument doc(filename);
	bool loadOkay = doc.LoadFile();

	if (!loadOkay)
	{
		std::cout << "Could not load file: " << filename << "Error = " << doc.ErrorDesc() << std::endl;
		return false;
	}

	TiXmlNode* pRoot = doc.FirstChild("Scene");
	for (TiXmlNode* pNode = pRoot->FirstChild(); pNode; pNode = pNode->NextSibling())
	{
        if (pNode->Value() == std::string("Material"))
        {
			TiXmlAttribute* pAtt = pNode->ToElement()->FirstAttribute();
			int index = pAtt->IntValue(); // get material index

			
            //
            // read reflectance coefficients
            //
            float amb[3], dif[3], spe[3], mir[3];
			float phongExp;
			for (TiXmlNode* pChild = pNode->FirstChild(); pChild; pChild = pChild->NextSibling())
			{
				if (pChild->Value() == std::string("Ambient"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &amb[0], &amb[1], &amb[2]);
					
				}
				else if (pChild->Value() == std::string("Diffuse"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &dif[0], &dif[1], &dif[2]);
				}
				else if (pChild->Value() == std::string("Specular"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f %f", &spe[0], &spe[1], &spe[2], &phongExp);
				}
				else if (pChild->Value() == std::string("Reflectance"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &mir[0], &mir[1], &mir[2]);
				}
			}
		Material mat2;
		mat2.matid=index;
		for(int i=0;i<3;i++){
			mat2.ambient[i]=amb[i];
			mat2.diffuse[i]=dif[i];
			mat2.specular[i]=spe[i];
            mat2.reflectance[i]=mir[i];

		}
		mat2.specular[3]=phongExp;
		mat.push_back(mat2);
            //
            // TODO: Save the scanned values for the current material in your data structures.
            //

        }
		else if (pNode->Value() == std::string("Vertex"))
        {
			TiXmlAttribute* pAtt = pNode->ToElement()->FirstAttribute();
			int index = pAtt->IntValue(); // get vertex index

			float coords[3];
			TiXmlNode* pChild = pNode->FirstChild();
			sscanf(pChild->FirstChild()->Value(), "%f %f %f", &coords[0], &coords[1], &coords[2]);
			Vertex vert2;
			vert2.vertexid=index;
            for(int i=0;i<3;i++)
			vert2.coor[i]=coords[i];
			vert.push_back(vert2);
			
            //
            // TODO: Save the scanned values for the current vertex in your data structures.
            //

		}
		else if (pNode->Value() == std::string("Triangle"))
        {
			TiXmlAttribute* pAtt = pNode->ToElement()->FirstAttribute();
			int index = pAtt->IntValue(); // get triangle index

			int vIndex[3], mIndex;
			for (TiXmlNode* pChild = pNode->FirstChild(); pChild; pChild = pChild->NextSibling())
			{
				if (pChild->Value() == std::string("Vertices"))
				{
					sscanf(pChild->FirstChild()->Value(), "%d %d %d", &vIndex[0], &vIndex[1], &vIndex[2]);
				}
				else if (pChild->Value() == std::string("MaterialId"))
				{
					sscanf(pChild->FirstChild()->Value(), "%d", &mIndex);
				}
			}

            //
            // TODO: Save the scanned values for the current triangle in your data structures.
            //
                Triangle tria;
                tria.triangleid=index;
                tria.vertexlerid[0]=vIndex[0];
                tria.vertexlerid[1]=vIndex[1];
                tria.vertexlerid[2]=vIndex[2];
                tria.materialid=mIndex;
                tri.push_back(tria);



		}
		else if (pNode->Value() == std::string("Sphere"))
        {
			TiXmlAttribute* pAtt = pNode->ToElement()->FirstAttribute();
			int index = pAtt->IntValue(); // Sphere index

			int vIndex, mIndex;
			float rad;
			for (TiXmlNode* pChild = pNode->FirstChild(); pChild; pChild = pChild->NextSibling())
			{
				if (pChild->Value() == std::string("Center"))
				{
					sscanf(pChild->FirstChild()->Value(), "%d", &vIndex);
				}
				else if (pChild->Value() == std::string("MaterialId"))
				{
					sscanf(pChild->FirstChild()->Value(), "%d", &mIndex);
				}
				else if (pChild->Value() == std::string("Radius"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f", &rad);
				}
			}


            Sphere sph1;
            sph1.centerid=vIndex;
            sph1.radius=rad;
            sph1.sphereid=index;
            sph1.material=mIndex;
            sph.push_back(sph1);



            //
            // TODO: Save the scanned values for the current sphere in your data structures.
            //

		}
		else if (pNode->Value() == std::string("PointLight"))
        {
			TiXmlAttribute* pAtt = pNode->ToElement()->FirstAttribute();
			int index = pAtt->IntValue(); // Light index

			float pos[3], intensity[3];
			for (TiXmlNode* pChild = pNode->FirstChild(); pChild; pChild = pChild->NextSibling())
			{
				if (pChild->Value() == std::string("Position"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &pos[0], &pos[1], &pos[2]);
				}
				else if (pChild->Value() == std::string("Intensity"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &intensity[0], &intensity[1], &intensity[2]);
				}
			
			}
			PointLight sa;
			sa.plightid=index;
		for(int i=0;i<3;i++){			
		sa.pos[i]=pos[i];
		sa.inten[i]=intensity[i];}
		polight.push_back(sa);
		
            //
            // TODO: Save the scanned values for the current point light in your data structures.
            //
		}
		else if (pNode->Value() == std::string("AmbientLight"))
        {
			float intensity[3];
			TiXmlNode* pChild = pNode->FirstChild();
			sscanf(pChild->Value(), "%f %f %f", &intensity[0], &intensity[1], &intensity[2]);
		for(int i=0;i<3;i++)
		alight.inten[i]=intensity[i];

            //
            // TODO: Save the scanned values for the ambient light in your data structures.
            //
		}
		else if (pNode->Value() == std::string("BackgroundColor"))
        {
            float bgColor[3];
			TiXmlNode* pChild = pNode->FirstChild();
			sscanf(pChild->Value(), "%f %f %f", &bgColor[0], &bgColor[1], &bgColor[2]);
            for(int i=0;i<3;i++)
            color1.color[i]=bgColor[i];
            //
            // TODO: Save the scanned values for the background color in your data structures.
            //
		}
		else if (pNode->Value() == std::string("RayReflectionCount"))
        {
            int rayReflectCount;
			TiXmlNode* pChild = pNode->FirstChild();
			sscanf(pChild->Value(), "%d", &rayReflectCount);
			color1.rayref=rayReflectCount;
            //
            // TODO: Save the scanned values for the ray reflection count in your data structures.
            //
		}
		else if (pNode->Value() == std::string("Camera"))
        {
			TiXmlAttribute* pAtt = pNode->ToElement()->FirstAttribute();
			int index = pAtt->IntValue(); // Camera index

			float gaze[3], up[3], pos[3];
			float left, right, bottom, top, distance;
			int nx, ny;
			std::string imageName;
			for (TiXmlNode* pChild = pNode->FirstChild(); pChild; pChild = pChild->NextSibling())
			{
				if (pChild->Value() == std::string("Position"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &pos[0], &pos[1], &pos[2]);
				}
				else if (pChild->Value() == std::string("Gaze"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &gaze[0], &gaze[1], &gaze[2]);
				}
				else if (pChild->Value() == std::string("Up"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f", &up[0], &up[1], &up[2]);
				}
				else if (pChild->Value() == std::string("ImagePlane"))
				{
					sscanf(pChild->FirstChild()->Value(), "%f %f %f %f %f %d %d", &left, &right, &bottom, &top, &distance, &nx, &ny);
				}
				else if (pChild->Value() == std::string("OutputName"))
				{
					imageName = pChild->FirstChild()->Value();
				}
			}
            Camera cam1;
            cam1.camid=index;
            for(int i=0;i<3;i++){
            cam1.gaze[i]=gaze[i];
            cam1.position[i]=pos[i];
            cam1.up[i]=up[i];}
            cam1.plane.bottom=bottom;
            cam1.plane.right=right;
            cam1.plane.left=left;
            cam1.plane.top=top;
            cam1.plane.distance=distance;
            cam1.plane.nx=nx;
            cam1.plane.ny=ny;
	    cam1.filename=imageName;
            cam.push_back(cam1);

            //
            // TODO: Save the scanned values for the current camera in your data structures.
            //

		}
	}

    return true;
}
