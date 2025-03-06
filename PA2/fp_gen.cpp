#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <utility>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;
#define SEED chrono::high_resolution_clock::now().time_since_epoch().count()

class RandomFloorplanGenerator
{
public:
    RandomFloorplanGenerator(int numBlocks, double minAspectRatio, double maxAspectRatio, double avgArea) : numBlocks(numBlocks), minAspectRatio(minAspectRatio), maxAspectRatio(maxAspectRatio), avgArea(avgArea) {}

    void generateFloorplan(fstream &fs)
    {
        mt19937 generator(SEED);
        uniform_real_distribution<double> aspectRatioDist(minAspectRatio, maxAspectRatio);

        vector<pair<int, int>> blocks;

        for (int i = 0; i < numBlocks; ++i)
        {
            double aspectRatio = aspectRatioDist(generator);

            int width = round(sqrt(avgArea / aspectRatio));
            int height = round(avgArea / width);

            width = adjustDimension(width);
            height = adjustDimension(height);

            while ((double)height / width < minAspectRatio || (double)height / width > maxAspectRatio)
            {
                if ((double)height / width < minAspectRatio)
                {
                    width -= 5; // Decrease width to increase the aspect ratio
                }
                else
                {
                    height -= 5; // Decrease height to reduce the aspect ratio
                }

                // Recalculate to maintain the area close to the average area
                width = adjustDimension(round(sqrt(avgArea / aspectRatio)));
                height = adjustDimension(round(avgArea / width));
            }

            blocks.push_back(make_pair(width, height));
        }

        fs << "NumBlocks " << numBlocks << endl;
        fs << "MinAspectRatio " << minAspectRatio << endl;
        fs << "MaxAspectRatio " << maxAspectRatio << endl;
        for (int i = 0; i < blocks.size(); ++i)
        {
            fs << "block_" << i << ' ' << blocks[i].first << ' ' << blocks[i].second << endl;
        }
    }

private:
    int numBlocks;
    double minAspectRatio, maxAspectRatio, avgArea;

    // Adjusts width or height to end with 0 or 5
    int adjustDimension(int dimension)
    {
        while (dimension % 10 != 0 && dimension % 10 != 5)
        {
            ++dimension; // Increment until we find a number that satisfies the condition.
        }
        return dimension;
    }
};

int main()
{

    int numBlocks;
    double minAspectRatio, maxAspectRatio;

    // User input for number of blocks and aspect ratio limits
    cout << "enter numblocks, min aspect ratio, and max aspect ratio:" << endl;
    cin >> numBlocks >> minAspectRatio >> maxAspectRatio;

    // Assume average area for each block is 550000
    double avgArea = 550000;

    fstream fout;
    string fileName = "floorplan_" + to_string(numBlocks) + ".txt";
    fout.open(fileName, fstream::out);
    if (!fout.is_open())
    {
        cout << "Error: the output file is not opened!!" << endl;
        exit(1);
    }

    RandomFloorplanGenerator generator(numBlocks, minAspectRatio, maxAspectRatio, avgArea);
    generator.generateFloorplan(fout);

    return 0;
}