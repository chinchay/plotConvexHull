/***********************************************************************
* Program:
*    extract.cpp
* Author:
*    Carlos Leon, MSG-BYU
* Summary:
*    This code takes data from a configuration file "relaxed.cfg". The data
*    taken are the number of elements in a configuration, its total energy in
*    eV, and its ID, and the values are saved in "out.scv"
*
*************************************************************************/

//#include "configuration.h"
#include <iostream>
#include <fstream>  // reading files
#include <sstream>      //  used for  strinstream

#include <iomanip>

#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <vector>
using namespace std;

#include <tgmath.h>

void write(string &fileOut, int componentsRate[], float energy)
{
   ofstream fout(fileOut.c_str());

   int sumComponents = componentsRate[0] + componentsRate[1] + componentsRate[2];
   fout << componentsRate[0]/sumComponents << "\t";
   fout << componentsRate[1]/sumComponents << "\t";
   fout << componentsRate[2]/sumComponents << "\t";
   fout << energy << std::endl;

   fout.close();
   return;
}


/**************************************************************************************
 * Determines if a configuration is made up of just one of the participant elements:
 * or Co, or Ni, or Ti. It saves the element by which the configuration is made up in
 * `pureOf`
 *************************************************************************************/
bool isPure(int componentsRate[], string &pureOf)
{
   if ( (componentsRate[1] == 0) && (componentsRate[2] == 0) ) // Co pure
   {
      pureOf = "Co";
      return true;
   }
   else if ( (componentsRate[0] == 0) && (componentsRate[2] == 0) ) // Ni pure
   {
      pureOf = "Ni";
      return true;
   }
   if ( (componentsRate[0] == 0) && (componentsRate[1] == 0) ) // Ti pure
   {
      pureOf = "Ti";
      return true;
   }
   pureOf = "None";
   return false;
}

/**************************************************************************************
 * Determines if a configuration is made up of just one of the participant elements:
 * or Co, or Ni, or Ti. It saves the element by which the configuration is made up in
 * `pureOf`
 *************************************************************************************/
void writePure(int componentsRate[], float totalEnergy, string confID,
               string &pureOf, ofstream &foutPure)
{
   foutPure.setf(ios::fixed);
   foutPure.setf(ios::showpoint);
   foutPure.precision(5);

   int nTotal = componentsRate[0] + componentsRate[1] + componentsRate[2];
   foutPure << componentsRate[0] << ", ";
   foutPure << componentsRate[1] << ", ";
   foutPure << componentsRate[2] << ", ";
   foutPure << totalEnergy / nTotal << ", ";
   foutPure << "eV/atom" << ", ";
   foutPure << pureOf << ", ";
   foutPure << confID << endl;

   return;
}


void write081(ofstream &fout081,
              string stringVectorA, string stringVectorB, string stringVectorC )
{
   static int k = 0;

   k++;

   fout081 << k << endl;
   fout081 << stringVectorA << endl;
   fout081 << stringVectorB << endl;
   fout081 << stringVectorC << endl;
   fout081 << endl;
   return;
}


void openFiles(ifstream &fin)
{
//   fin( fileName.c_str() );
   if (fin.fail())
   {
      cout << "I could not open the file" << endl;
      return;
   }
   return;
}

void extractFromFile(ifstream &fin, ofstream &fout, ofstream &foutPure, ofstream &fout081)
{
   fout << "Co, Ni, Ti, Energy, conf_id" << endl; // head
   foutPure << "Co, Ni, Ti, Etotal, units, pureOf, confID" << endl; // head

   string line;

   string stringVectorA;
   string stringVectorB;
   string stringVectorC;

   int size;
   float energy = 0;
   int componentsRate[3];
   int id;
   int type;
   float cx;
   float cy;
   float cz;
   float fx;
   float fy;
   float fz;
   float x;
   float y;
   float z;
   float xx;
   float yy;
   string pureOf;
   int sumComponents;
   string tempStringA;
   string tempStringB;
   string confID;

//   for (int j = 1; j < 195484 ; j++)
   int j = 0;
   while (!fin.eof())
   {
      getline(fin, line);
      if (line == "BEGIN_CFG")
      {
         j++;
         getline(fin, line); //"Size"

         fin >> size;
         fin.ignore();
         // "Supercell" line + 3 vectors lines
         getline(fin, line); // "Supercell"

         getline(fin, stringVectorA);
         getline(fin, stringVectorB);
         getline(fin, stringVectorC);
//         getline(fin, line);
//         getline(fin, line);
//         getline(fin, line);

         getline(fin, line); // head line: "AtomData: id type ..."

         // IDs:
         componentsRate[0] = 0;
         componentsRate[1] = 0;
         componentsRate[2] = 0;

         for (int i = 0; i < size; i++)
         {
            fin >> id >> type >> cx >> cy >> cz >> fx >> fy >> fz;
            fin.ignore();

            if (type == 0)
               componentsRate[0] += 1;
            else if (type == 1)
               componentsRate[1] += 1;
            else if (type == 2)
               componentsRate[2] += 1;
         }

         getline(fin, line); // "Energy"
         fin >> energy;
         fin.ignore();

         getline(fin, line);
         getline(fin, line);
         getline(fin, line);
         getline(fin, line);

         fin >> tempStringA >> tempStringB >> confID;
         fin.ignore();


         //
         while (line != "END_CFG")
         {
            getline(fin, line);
//            cout << line << endl;
         }

         if ( (componentsRate[0] == 0) &&
              (componentsRate[1] == 8) &&
              (componentsRate[2] == 1)    )
         {
            write081(fout081, stringVectorA, stringVectorB, stringVectorC );
         }


      }

//      write(fileOut, componentsRate, energy);
      cout << "j = " << j << endl; // "  conf_id = " << confID << endl;
      getline(fin, line); // just for catching an empty line between configs.
//      cin.ignore();


      sumComponents = componentsRate[0] + componentsRate[1] + componentsRate[2];

      fout.setf(ios::fixed);
      fout.setf(ios::showpoint);
      fout.precision(4);


      if ( isPure(componentsRate, pureOf) )
         writePure(componentsRate, energy, confID, pureOf, foutPure);

      fout.setf(ios::fixed);
      fout.setf(ios::showpoint);
      fout.precision(5);

      fout << componentsRate[0] << ", ";
      fout << componentsRate[1] << ", ";
      fout << componentsRate[2] << ", ";
      fout << energy << ", " << confID <<endl;


//      x = 100.0 * (float)componentsRate[0] / sumComponents;
//      y = 100.0 * (float)componentsRate[1] / sumComponents;
//      z = 100.0 * (float)componentsRate[2] / sumComponents;
//      fout << x << ", " << y << ", " << z << ", " << energy << endl;

//      xx = x + (y * tan( 30.0 * 3.14159 / 180.0) );
//      yy = y;
//      fout << xx << ", " << yy << ", " << energy << "," << endl;


   }




}

int main()
{
   string fileName = "relaxed.cfg";
   string fileOut = "out.csv";
   string fileOutPure = "outPure.csv";
   string file081 = "081.csv";


   ifstream fin(fileName);
   ofstream fout(fileOut);
   ofstream foutPure(fileOutPure);
   ofstream fout081(file081);

   openFiles(fin);


   cout << "I am going to extract data:" << endl;
   extractFromFile(fin, fout, foutPure, fout081);

   fin.close();
   fout.close();
   foutPure.close();
   fout081.close();


   return 0;
}
