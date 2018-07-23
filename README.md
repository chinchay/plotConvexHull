# plotConvexHull
Plot a convex hull using plotly.

Procedure:
1. Data of structures are taken from a file called “relaxed.cfg” containing the configurations in the .cfg format
2. Run extract.cpp, to get a file called “out.csv”
3. Run the python code.
4. Modify the value of “numberOfPoints” in the python file according to the number of configurations we are dealing with.
5. Adjust conveniently the value of “reference” in the “isEnergyAbove()” function in case the convex hull takes a simplex from the top. Just increase or decrease it by a few meV.
