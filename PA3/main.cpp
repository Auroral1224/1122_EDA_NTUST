#define IL_STD
#include <ilcplex/ilocplex.h>

#include "PA3.h"

ILOSTLBEGIN

int main(int argc, char **argv) {
    fstream fin;
    /* open the input file */
    checkInputFile(fin, argc, argv);

    /* parse input file */
    int grid_x;
    int grid_y;
    int numHoles;
    vector<hole> allHoles;
    parseInputFile(fin, grid_x, grid_y, numHoles, allHoles);

    IloEnv env;
    try {
        /* Initialization */
        IloModel model(env);
        IloCplex cplex(model);

        /* Define decision variables */
        // a set of conflict edges
        vector<edge> CE;
        for (int i = 0; i < numHoles - 1; ++i) {
            for (int j = i + 1; j < numHoles; ++j) {
                if (isConflict(allHoles[i], allHoles[j])) {
                    edge conflictEdge(allHoles[i], allHoles[j]);
                    CE.push_back(conflictEdge);
                    // cout << "conflict: " << setw(4) << allHoles[i].hole_id << setw(4) << allHoles[j].hole_id << endl;
                }
            }
        }

        // binary variables, cij = 1 if cij1 == 1 && cij2 == 1;
        // cijk = 1 if xik = xjk, = 0 otherwise
        IloNumVarArray cij(env), cij1(env), cij2(env);
        // objective function = sigma(cij) for every element in CE
        IloExpr exprObj(env);
        for (int i = 0; i < CE.size(); ++i) {
            cij.add(IloNumVar(env, 0, 1, ILOINT));
            cij1.add(IloNumVar(env, 0, 1, ILOINT));
            cij2.add(IloNumVar(env, 0, 1, ILOINT));
            exprObj += cij[i];
        }

        // {x1, x2} = {0, 0}: first mask, {0, 1}: second mask, {1, 0}: third mask
        IloNumVarArray x1(env), x2(env);
        for (int i = 0; i < numHoles; ++i) {
            x1.add(IloNumVar(env, 0, 1, ILOINT));
            x2.add(IloNumVar(env, 0, 1, ILOINT));
        }

        // Objective function
        model.add(IloMinimize(env, exprObj));

        /* add constraints */
        IloRangeArray con(env);

        // three available colors
        for (int i = 0; i < numHoles; ++i) {
            con.add(x1[i] + x2[i] <= 1);  // nth mask, for not (1, 1)
        }

        // CE constraints
        for (int i = 0; i < CE.size(); ++i) {
            con.add((x1[CE[i].vi.hole_id] + x1[CE[i].vj.hole_id] - cij1[i]) <= 1);              // for {xi1, xj1} = (1, 1)
            con.add(((1 - x1[CE[i].vi.hole_id]) + (1 - x1[CE[i].vj.hole_id]) - cij1[i]) <= 1);  // for {xi1, xj1} = (0, 0)
            con.add((x2[CE[i].vi.hole_id] + x2[CE[i].vj.hole_id] - cij2[i]) <= 1);              // for {xi2, xj2} = (1, 1)
            con.add(((1 - x2[CE[i].vi.hole_id]) + (1 - x2[CE[i].vj.hole_id]) - cij2[i]) <= 1);  // for {xi2, xj2} = (0, 0)
            con.add((cij1[i] + cij2[i] - cij[i]) <= 1);                                         // if  {cij1, cij2} == (1, 1) then cij = 1
        }
        model.add(con);

        /* Solve the problem */
        cplex.solve();

        /* Print solution */
        if (cplex.getStatus() == IloAlgorithm::Infeasible) {
            env.out() << "no Solution" << endl;
        }
        assert(cplex.getStatus() == IloAlgorithm::Feasible || cplex.getStatus() == IloAlgorithm::Optimal);

        env.out() << "Solution status = " << cplex.getStatus() << endl;
        env.out() << "Total conflict = " << cplex.getObjValue() << endl;

        for (int i = 0; i < numHoles; ++i) {
            env.out() << left << setw(4) << i << right << setw(4) << cplex.getValue(x1[i]) << setw(4) << cplex.getValue(x2[i]) << endl;
        }

        for (int i = 0; i < cij.getSize(); ++i) {
            bool conflict = static_cast<bool>(cplex.getValue(cij[i]));
            if (conflict) {
                env.out() << CE[i].vi.hole_id << " conflict with " << CE[i].vj.hole_id << endl;
            }
        }

        /* open the output file */
        fstream fout;
        fout.open(argv[2], fstream::out);
        if (!fout.is_open()) {
            cout << "Error: the output file is not opened!!" << endl;
            exit(1);
        }

        fout << cplex.getObjValue() << endl;

        for (int i = 0; i < numHoles; ++i) {
            // allHoles[i].layer = printMask(cplex.getValue(x1[i]), cplex.getValue(x2[i]));
            // fout << allHoles[i].hole_id << ' ' << allHoles[i].layer << endl;
            fout << allHoles[i].hole_id << ' ' << printMask(cplex.getValue(x1[i]), cplex.getValue(x2[i])) << endl;
        }

    } catch (IloException &ex) {
        cerr << "Error: " << ex << endl;
    }
    env.end();

    // cout << "\nDouble check!" << endl;
    // for (int i = 0; i < numHoles - 1; ++i) {
    //     for (int j = i + 1; j < numHoles; ++j) {
    //         if (isConflict(allHoles[i], allHoles[j]) && (allHoles[i].layer == allHoles[j].layer)) {
    //             cout << "conflict: " << setw(4) << allHoles[i].hole_id << setw(4) << allHoles[j].hole_id << endl;
    //         }
    //     }
    // }

    return 0;
}
