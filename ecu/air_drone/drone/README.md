Getting Started:
----------------

```python
>>> import libardrone
>>> drone = libardrone.ARDrone()
>>> # You might need to call drone.reset() before taking off if the drone is in
>>> # emergency mode
>>> drone.takeoff()
>>> drone.land()
>>> drone.halt()
```

The drone's property `image` contains always the latest image from the camera.
At present this is in the format of a numpy array with dimensions (width, height, 3).
This may change in future.
The drone's property `navdata` contains always the latest navdata.


Demo:
-----

There is also a demo application included which shows the video from the drone
and lets you remote-control the drone with the keyboard:

    RETURN      - takeoff
    SPACE       - land
    BACKSPACE   - reset (from emergency)
    a/d         - left/right
    w/s         - forward/back
    1,2,...,0   - speed
    UP/DOWN     - altitude
    LEFT/RIGHT  - turn left/right

Here is a [video] of the library in action:

  [video]: http://youtu.be/2HEV37GbUow

Repository:
-----------

The public repository is located here for the AR.Drone 1.0:

  git://github.com/venthur/python-ardrone.git

At present the AR.Drone 2.0 has a separate fork here:

  git://github.com/adetaylor/python-ardrone.git

Requirements:
-------------

This software was tested with the following setup:

  * Python 2.7.6
  * Unmodified AR.Drone firmware 2.0


License:
--------

This software is published under the terms of the MIT License:

  http://www.opensource.org/licenses/mit-license.php

