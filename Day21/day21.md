<h1>Part 1</h1>
This one came out the day before I went on a boat without an internet connection, so I got to think about this while bobbing on the waves at night!

I used a top-down approach - I first used Manhatten distance to calculate the amount of button presses needed to move the first directional robot between any two keys on the directional keypad (including the keypress required for the robot to press the key).

Then I used Dijkstra to calculate the same thing but for the second directional robot.

Similarly, Dijkstra can be used to calculate the button presses for the digit keypad robot. I used Dijkstra to individually calculate the number of button presses required between each of the digit keys that it must reach. This is possible because all other robots must end up over the 'A' key when the digit keypad actually presses the key, so the state always resets.

<h1>Part 2</h1>
My approach only required minor tweaks to work for part 2! I created a struct to hold the unordered maps that stored the button presses.

It would have been possible to just use two unordered maps since the ones for higher levels will no longer be needed later.