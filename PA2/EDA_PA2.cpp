#include "slicing_tree.h"
/*
    input data, macro name must be "block_**" (** indicates index, starting from 0),
    otherwise this program won't work due to parser error.
*/

int main(int argc, char **argv)
{
    /* open the input file */
    if (argc != 3)
    {
        cout << "Usage: ./[exe] [input file] [output file]" << endl;
        exit(1);
    }

    int numBlocks;
    double minAR, maxAR;

    fstream fin;
    fin.open(argv[1], fstream::in);
    if (!fin.is_open())
    {
        cout << "Error: the input file is not opened!!" << endl;
        exit(1);
    }

    /* record timestamp */
    chrono::_V2::system_clock::time_point time1 = chrono::system_clock::now();

    /*  parse the input file */
    cout << "======================" << endl;
    cout << "        Parser        " << endl;
    cout << "======================" << endl;

    numBlocks = parseInfo<int>(fin, "NumBlocks");
    minAR = parseInfo<double>(fin, "MinAspectRatio");
    maxAR = parseInfo<double>(fin, "MaxAspectRatio");
    allMacros A(numBlocks, minAR, maxAR);
    A.parseData(fin);

    bool isPartition;         // to determine whether is partition method or not
    int partition_amount = 0; // partition amount, only support 2x2, 3x3, 4x4, and 5x5.
    double solution_quality;
    ifPartition(numBlocks, isPartition, partition_amount, solution_quality);

    /*
    init_mode (enum)
        V     : 12V3V4V...
        H     : 12H3H4H...
        VH_alt: 12V3H4V... (V/H alternative)
    */
    // (allMacros, initial mode)
    Expression E(A, init_mode::VH_alt); // Macros -> initial condition of polish expression

    // (allMacros, slice count)
    // divide and conquer
    vector<Expression> EE = E.partition(A, partition_amount, init_mode::VH_alt, isPartition);
    vector<Expression> bestExpressions(partition_amount);

#if DEBUG
    cout << "vvvvvvvvvv---------------initial expression:---------------vvvvvvvvvv" << endl;
    cout << E << endl;
    cout << "^^^^^^^^^^-------------------------------------------------^^^^^^^^^^" << endl;
#endif

    cout << "numBlocks = " << numBlocks << ", initial condition spec: " << endl;
    E.printSpec(A);
    cout << endl;
    cout << "starting SA, press CTRL + C or wait 10 minutes to interrupt..." << endl;
    try
    {
        if (isPartition)
        {
            int completion = 0;
            vector<bool> completed(partition_amount, false);
            while (completion < partition_amount)
            {
                for (int i = 0; i < EE.size(); ++i)
                {
                    if (!completed[i])
                    {
                        cout << "Partition SA, " << completion << " / " << partition_amount << " completed." << endl;
                        cout << "Try partition #" << i + 1 << "..." << endl;
                        completed[i] = EE[i].simulatedAnnealing(A, isPartition, solution_quality);
                        if (completed[i])
                        {
                            ++completion;
                            cout << "partition #" << i + 1 << " completed, spec: " << endl;
                            bestExpressions[i] = EE[i];
                            EE[i].printSpec(A);
                            cout << endl;
                        }
                        else if (bestExpressions[i].cost(A) < EE[i].cost(A))
                        {
                            cout << "NG, but updating current best, spec: " << endl;
                            bestExpressions[i] = EE[i];
                            EE[i].printSpec(A);
                            cout << endl;
                        }
                        else
                        {
                            cout << "NG, spec: " << endl;
                            EE[i].printSpec(A);
                            cout << endl;
                        }
                    }
                }
            }
        }
        else
        {
            // not partition method, keep retry until 10min, ctrl + C or cost >= solution quality.
            E.simulatedAnnealing(A, isPartition, solution_quality);
        }
        cout << "\nfinshed.\n\n final result:\n\n";
    }
    catch (const runtime_error &e) // ctrl + c during SA
    {
        cout << "\nctrl + C detected. SA interrupted.\n\nbest result:\n\n";
    }
    catch (const length_error &e) // 10 min
    {
        cout << "\n10 minutes exceeded. SA interrupted.\n\nbest result:\n\n";
    }

    if (isPartition)
    {
        if (ctrl_c_signal || timeout_flag)
        {
            cout << "combining local best due to interruption..." << endl;
            for (int i = 0; i < partition_amount; ++i)
            {
                cout << i + 1 << " / " << partition_amount << endl;
                if (!bestExpressions[i].size())
                {
                    cerr << "terminate too early, SA failed." << endl;
                    exit(1);
                }
            }
            E.combinePartition(bestExpressions, partition_amount); // combine partition result based on fixed expression
        }
        else
        {
            E.combinePartition(EE, partition_amount); // combine partition result based on fixed expression
        }
    }
    E.traceXY(A);   // trace XY of macro array A for optimal expression
    E.printSpec(A); // print spec

    /* open the output file */
    fstream fout;
    fout.open(argv[2], fstream::out);
    if (!fout.is_open())
    {
        cout << "Error: the output file is not opened!!" << endl;
        exit(1);
    }

/* output result */
#if DEBUG
    cout << "vvvvvvvvvv---------------optimal expression:---------------vvvvvvvvvv" << endl;
    cout << E << endl;
    cout << "^^^^^^^^^^-------------------------------------------------^^^^^^^^^^" << endl;
    cout << "details: (<name><width><height><x0><y0>)" << endl;
    cout << A << endl;
#endif
    E.outputData(A, fout, OUTPUT_FORMAT);

    /* get runtime info */
    chrono::_V2::system_clock::time_point time2 = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = time2 - time1;
    time_t start_time = chrono::system_clock::to_time_t(time1);
    time_t end_time = chrono::system_clock::to_time_t(time2);
    cout << "starting computation at " << ctime(&start_time);
    cout << "finished computation at " << ctime(&end_time);
    cout << "elapsed time: " << elapsed_seconds.count() << "s" << endl;

    /* finished */
    cout << "Finished, the output file's name is \"" << argv[2] << "\", press any key to exit..." << endl;
    cin.get();
    return 0;
}