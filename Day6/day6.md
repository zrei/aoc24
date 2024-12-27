<h1>Part 1</h1>
Traverse through the map, tracking the current position and direction to know how to move until we go out of bounds. Store each encountered position in an unordered map to find the number of unique points traversed.

<h1>Part 2</h1>
Since there are only a limited number of positions to try (positions that are "in front" of points on a path), I just brute forced by trying every point and seeing if we get stuck in a loop (encounters a point and direction while traversing that we encountered before).