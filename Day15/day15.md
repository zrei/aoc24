<h1>Part 1</h1>
There is a 2D matrix that describes the state of the warehouse (whether each point is a wall, box or empty). Track the current position of the robot and the positions. When it attempts to move, we do a few checks:
* If there is a wall in that direction, move is not possible so we remain still
* If there is a box in that direction, we calculate the final position that the final box (i.e. the final box in the line of boxes starting from the box directly next to the robot) will end up in. If that position is out of bounds or in a wall, moving is not possible so we remain still
* If there is a box and moving is possible, we move the robot and update the state of the matrix 
* If there is no box, just move the robot

Using the final state, calculate the final value.

<h1>Part 2</h1>
Uses the same approach, but the state matrix now also describes if it is a LEFT_BOX or a RIGHT_BOX. When checking if there are boxes to be moved, it now does a flood fill in the moving direction to add all boxes that will be moved - now all of their final states must be checked for validity.

When calculating the final value, use LEFT_BOX for the position of the box.