<h1>Part 1</h1>
First store all the points in the path. Then iterate through each point and check each possible shortcut starting from that point. If we end up in a wall, we skip this shortcut. If we don't, we know the original time of the new point and we can calculate the amount of time saved.

<h1>Part 2</h1>
Used a similar approach to part 1 but used BFS to calculate the possible shortcuts.