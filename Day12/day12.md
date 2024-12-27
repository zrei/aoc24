<h1>Part 1</h1>
Starting from each point in the matrix, a flood fill is used to move through the entire region connected to that point. The points found are registered in an unordered map, the size of which becomes the area. The perimeter is found by checking, for each point, whether each side is at the edge of the map or neighbouring a point that is not in the region (different character).

A 2D vector is used to track if we've checked a point before so we can skip it if we encounter it later.

<h1>Part 2</h1>
Only solved this after I flew home... The Reddit informed me that the number of corners is actually the same as the number of sides. The same approach for flood fill and area is used from part 1, except instead of checking for perimeter we check how many corners each point in the region is involved in.