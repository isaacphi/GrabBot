#!/usr/bin/env python

import Leap
import sys
import time
import math


GRIP_MIN_DIST = 40
GRIP_MAX_DIST = 90
GRIP_ANGLE_MAX = 30


def dist(v1, v2):
    return math.sqrt((v1[0] - v2[0]) ** 2 + (v1[1] - v2[1]) ** 2 + (v1[2] - v2[2]) ** 2)


class GripBotListener(Leap.Listener):

    def on_init(self, controller):
        print "Initialized"

    def on_connect(self, controller):
        print "Connected"

    def on_disconnect(self, controller):
        print "Disconnected"

    def on_exit(self, controller):
        print "Exited"

    def on_frame(self, controller):
        frame = controller.frame()

        if len(frame.hands) != 1:
            return

        hand = frame.hands[0]

        # pause if palm is up
        if hand.palm_normal[1] > 0:
            return

        thumb = hand.fingers[0].tip_position
        finger = hand.fingers[1].tip_position

        x = int(finger[0])
        y = -int(finger[2])
        distance = dist(finger, thumb)

        if distance < GRIP_MIN_DIST:
            angle = 0
        elif distance > GRIP_MAX_DIST:
            angle = GRIP_ANGLE_MAX
        else:
            angle = GRIP_ANGLE_MAX * (float(distance) - GRIP_MIN_DIST) / GRIP_MAX_DIST

        print x, y, angle


def main():
    listener = GripBotListener()
    controller = Leap.Controller()
    controller.add_listener(listener)

    print "Press Enter to quit..."
    try:
        sys.stdin.readline()
    except KeyboardInterrupt:
        pass
    finally:
        controller.remove_listener(listener)


if __name__ == "__main__":
    main()
