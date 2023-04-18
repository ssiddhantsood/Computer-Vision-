#include <iostream>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>

using namespace std;
using namespace cv;

const int fWidth = 800;
const int FRAME_HEIGHT = 600;
const int FRAME_RATE = 30;

float cubeScalingFactor = 50.0f;      
float tetrahedronScalingFactor = 100.0f;

Mat createFrame(const vector<Point3f>& cubeVertices, const vector<Point3f>& tetrahedronVertices, const Scalar& cubeColor, const Scalar& tetrahedronColor) {
    Mat frame = Mat::zeros(FRAME_HEIGHT, fWidth, CV_8UC3);

    vector<Point2f> projectedCubeVertices;
    vector<Point2f> projectedTetrahedronVertices;
    
    for (const auto& vertex : cubeVertices) {
        projectedCubeVertices.push_back(Point2f(vertex.x + fWidth / 2, vertex.y + FRAME_HEIGHT / 2));
    }
    for (const auto& vertex : tetrahedronVertices) {
        projectedTetrahedronVertices.push_back(Point2f(vertex.x + fWidth / 2, vertex.y + FRAME_HEIGHT / 2));
    }

    int lineThickness = 2; 

    vector<pair<int, int>> cubeEdges = {{0, 1}, {1, 3}, {3, 2}, {2, 0}, {4, 5}, {5, 7}, {7, 6}, {6, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};

    for (size_t i = 0; i < projectedCubeVertices.size(); i++) {
        circle(frame, projectedCubeVertices[i], 4, cubeColor, -1);
    }
    for (const auto& edge : cubeEdges) {
        line(frame, projectedCubeVertices[edge.first], projectedCubeVertices[edge.second], cubeColor, lineThickness);
    }

    vector<pair<int, int>> tetrahedronEdges = {{0, 1}, {1, 2}, {2, 0}, {0, 3}, {1, 3}, {2, 3}};

    for (size_t i = 0; i < projectedTetrahedronVertices.size(); i++) {
        circle(frame, projectedTetrahedronVertices[i], 4, tetrahedronColor, -1);
    }
    for (const auto& edge : tetrahedronEdges) {
        line(frame, projectedTetrahedronVertices[edge.first], projectedTetrahedronVertices[edge.second], tetrahedronColor, lineThickness);
    }

    return frame;
}

void saveCoordinates(const vector<Point3f>& vertices)
{
    ofstream coordFile("coordinates.txt", ios_base::app);
    for (const auto& vertex : vertices)
    {
        coordFile << "(" << vertex.x << "," << vertex.y << "," << vertex.z << "), ";
    }
    coordFile << endl;
    coordFile.close();
}

void part1(int frames)
{
    vector<Point3f> cubeVertices =
    {
        {1, 1, 1}, {1, 1, -1}, {1, -1, 1}, {1, -1, -1},
        {-1, 1, 1}, {-1, 1, -1}, {-1, -1, 1}, {-1, -1, -1}
    };

    vector<Point3f> tetrahedronVertices = 
    {
        {0, 1, sqrt(2)}, {1, -1, 0}, {-1, -1, 0}, {0, 0, -sqrt(2)}
    };

    
    for (auto& vertex : cubeVertices) {
        vertex *= cubeScalingFactor;
    }
    for (auto& vertex : tetrahedronVertices) {
        vertex *= tetrahedronScalingFactor;
    }

    VideoWriter video("rotation.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), FRAME_RATE, Size(fWidth, FRAME_HEIGHT));

    for (int frame = 0; frame < frames; ++frame) 
    {
        double angle = 2 * M_PI * frame / frames;

        Mat rotMatrix = (Mat_<float>(3, 3) <<
                                cos(angle), 0, sin(angle),
                                0, 1, 0,
                                -sin(angle), 0, cos(angle));

        vector<Point3f> transformedCubeVertices;
        for (const auto& vertex : cubeVertices) {
            Mat transformedVertex = rotMatrix * Mat(vertex, true);
            transformedCubeVertices.push_back(Point3f(transformedVertex));
        }

        if (frame < 4)
        {
            saveCoordinates(transformedCubeVertices);
        }

        vector<Point3f> transformedTetrahedronVertices;
        for (const auto& vertex : tetrahedronVertices) 
        {
            Mat transformedVertex = rotMatrix * Mat(vertex, true);
            transformedTetrahedronVertices.push_back(Point3f(transformedVertex));
        }

        Mat frameWithBothObjects = createFrame(transformedCubeVertices, transformedTetrahedronVertices, Scalar(0, 255, 0), Scalar(255, 0, 0));
        video.write(frameWithBothObjects);
    }
    
    video.release();
}

int main()
{
    int frames = 120;
    part1(frames);
}
