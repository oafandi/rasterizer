#include <iostream>
#include <vector>
#include "Scene.h"

using namespace std;

Scene *scene;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Please run the rasterizer as:" << endl
             << "\t./rasterizer <input_file_name>" << endl;
        return 1;
    }
    else
    {
        const char *xmlPath = argv[1];
        scene = new Scene(xmlPath);
        scene->render();
        return 0;
    }
}