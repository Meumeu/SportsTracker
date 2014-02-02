/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sportstracker 1.0
import "../util.js" as Util

CoverBackground {
    property Workout workout;
    property ApplicationWindow window;
    signal stopWorkout;
    signal startWorkout;


    Image {
        width: parent.width * 0.9
        x: parent.width * 0.05
        y: parent.width * 0.1
        height: sourceSize.height * width / sourceSize.width
        fillMode: Image.PreserveAspectFit
        source: Qt.resolvedUrl("../../images/cover.png")
        opacity: 0.1
    }

    CoverPlaceholder {
        Label {
            text: (workout.status == Workout.Tracking || workout.status == Workout.Paused) ?
                  "%1\n%2\n%3"
                    .arg(Util.timeToString(workout.duration))
                    .arg(Util.distanceToString(workout.distance))
                    .arg(Util.speedToString(workout.speed)) :
                  ""
            font.pixelSize: Theme.fontSizeLarge
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
        }
    }

    CoverActionList {
        enabled: workout.status == Workout.Tracking || workout.status == Workout.Paused

        CoverAction {
            iconSource: "image://theme/icon-camera-stop"
            onTriggered: stopWorkout()
        }

        CoverAction {
            iconSource: workout.status == Workout.Tracking ? "image://theme/icon-cover-pause" : "image://theme/icon-camera-record"
            onTriggered: workout.status = (workout.status == Workout.Tracking ? Workout.Paused : Workout.Tracking)
        }
    }

    CoverActionList {
        enabled: workout.status == Workout.NotStarted || workout.status == Workout.Stopped

        CoverAction {
            iconSource: "image://theme/icon-camera-sports"
            onTriggered: startWorkout();
        }
    }
}


