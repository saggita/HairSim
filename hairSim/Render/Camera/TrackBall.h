/*
 * (c) Copyright 1993, 1994, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * US Government Users Restricted Rights
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(TM) is a trademark of Silicon Graphics, Inc.
 */
/*
 * trackball.h
 * A virtual trackball implementation
 * Written by Gavin Bell for Silicon Graphics, November 1988.
 */

#ifndef TRACKBALL_HH
#define TRACKBALL_HH

#include "Camera.h"


/** A simple virtual trackball for rotational user input. The
    trackball modifies the camera eye position directly. Contributors
    include Gavin Bell, Adrian Secord, Yotam Gingold and Mikl&oacute;s
    Bergou. */
class TrackBall
{
public:
  enum TrackBallMode {
    TRACKBALL,                 ///< TrackBall mode
    GIMBAL,                    ///< Gimbal rotation mode
    NUM_ROTATION_MODES         ///< The number of modes
  };

  /// Default constructor.
  explicit TrackBall(Camera* c);

  /// Start a rotational mouse motion.
  /// Position in [-1,1] x [-1,1].
  void start(const Vec2& p);

  /// Update a rotational mouse motion with a new mouse point.
  /// Position in [-1,1] x [-1,1].
  void update(const Vec2& p);

  /// Stop a rotational mouse motion.
  void stop();

  /// Get the current rotation as a quaternion.
  void getRotation( scalar r[4]) const;

  /// Get the current rotation mode.
  TrackBallMode getTrackBallMode() const;

  /// Set the current rotation mode to TrackBall or Gimbal
  void setTrackBallMode(const TrackBallMode m);

protected:

  Camera* m_camera;                           ///< The current camera to obey.
  bool m_rotating;                            ///< Whether we are rotating.
  Vec2 m_startPos;                           ///< Start of a rotational drag.
  scalar m_rotation[4];                       ///< Current user rotation.
  TrackBallMode m_mode;                       ///< Current rotation type.
};

#endif // TRACKBALL_HH