/*
 * Maps hex topology onto LED's
 * (C) Voidstar Lab LLC 2021
 */

#ifndef MAPPING_H_
#define MAPPING_H_

#include "../../globals.h"

namespace Chromance
{
    static float Fmap(float x, float inMin, float inMax, float outMin, float outMax)
    {
        return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }

    static const int32_t NumberOfSegments = 40;
    static const int32_t NumberOfNodes = 25;
    /**
     * Border nodes are on the very edge of the network.
     * Ripples fired here don't look very impressive.
     */ 
    static const int32_t NumberOfBorderNodes = 10;
    static const int32_t BorderNodes[NumberOfBorderNodes] = {0, 1, 2, 3, 6, 10, 13, 19, 21, 24};
    /**
     * Cube nodes link three equiangular segments
     * Firing ripples that always turn in one direction will draw a cube
    */
    static const int32_t NumberOfCubeNodes = 8;
    static const int32_t CubeNodes[NumberOfCubeNodes] = {7, 8, 9, 11, 12, 17, 18, 20};
    static const int32_t NumberOfFunNodes = 6;
    static const int32_t FunNodes[] = {4, 5, 14, 15, 16, 22, 23};
    // Firing ripples that always turn in one direction will draw a starburst
    static const int32_t StarBurstNode = 15;
    /**
     * LED segment numbers
     * Beam 0 is at 12:00 and advance clockwise
     * -1 means nothing connected on that side
     * Index stands for the node ie nodeConnections[7] stands for node 7
     */
    static const int32_t NodeConnections[NumberOfNodes][6] =
    {
        {-1, -1, 1, -1, 0, -1},
        {-1, -1, 3, -1, 2, -1},
        {-1, -1, 5, -1, 4, -1},
        {-1, 0, 6, 12, -1, -1},
        {-1, 2, 8, 14, 7, 1},

        {-1, 4, 10, 16, 9, 3},
        {-1, -1, -1, 18, 11, 5},
        {-1, 7, -1, 13, -1, 6},
        {-1, 9, -1, 15, -1, 8},
        {-1, 11, -1, 17, -1, 10},

        {12, -1, 19, -1, -1, -1},
        {14, -1, 21, -1, 20, -1},
        {16, -1, 23, -1, 22, -1},
        {18, -1, -1, -1, 24, -1},
        {13, 20, 25, 29, -1, 19},

        {15, 22, 27, 31, 26, 21},
        {17, 24, -1, 33, 28, 23},
        {-1, 26, -1, 30, -1, 25},
        {-1, 28, -1, 32, -1, 27},
        {29, -1, 34, -1, -1, -1},

        {31, -1, 36, -1, 35, -1},
        {33, -1, -1, -1, 37, -1},
        {30, 35, 38, -1, -1, 34},
        {32, 37, -1, -1, 39, 36},
        {-1, 39, -1, -1, -1, 38}
    };
    /**
     * First member: Node closer to ceiling
     * Second: Node closer to floor
     * Node connection list ()
     */
    static const int32_t SegmentConnections[NumberOfSegments][2] =
    {
        {0, 3},
        {0, 4},
        {1, 4},
        {1, 5},
        {2, 5},
        {2, 6},
        {3, 7},
        {4, 7},
        {4, 8},
        {5, 8},
        {5, 9},
        {6, 9},
        {3, 10},
        {7, 14},
        {4, 11},
        {8, 15},
        {5, 12},
        {9, 16},
        {6, 13},
        {10, 14},
        {11, 14},
        {11, 15},
        {12, 15},
        {12, 16},
        {13, 16},
        {14, 17},
        {15, 17},
        {15, 18},
        {16, 18},
        {14, 19},
        {17, 22},
        {15, 20},
        {18, 23},
        {16, 21},
        {19, 22},
        {20, 22},
        {20, 23},
        {21, 23},
        {22, 24},
        {23, 24}
    };
    // These are mapped as base 1 and NOT base 0, hence the S - 1
    #define headof(S) ((S - 1) * 14)
    #define tailof(S) (headof(S) + 13)
    /**
     * First member: Strip number
     * Second: LED index closer to ceiling
     * Third: LED index closer to floor
     */
    static const int32_t LEDAssignments[NumberOfSegments][3] =
    {
        {RedStripIndex, headof(3), tailof(3)},
        {RedStripIndex, tailof(2), headof(2)},
        {GreenStripIndex, headof(10), tailof(10)},
        {GreenStripIndex, tailof(9), headof(9)},
        {GreenStripIndex, headof(4), tailof(4)},
        {GreenStripIndex, tailof(3), headof(3)},

        {RedStripIndex, tailof(6), headof(6)},
        {BlackStripIndex, tailof(11), headof(11)},
        {GreenStripIndex, headof(11), tailof(11)},
        {GreenStripIndex, tailof(8), headof(8)},
        {GreenStripIndex, headof(12), tailof(12)},
        {BlueStripIndex, tailof(11), headof(11)},

        {RedStripIndex, headof(4), tailof(4)},
        {BlackStripIndex, tailof(10), headof(10)},
        {RedStripIndex, tailof(1), headof(1)},
        {GreenStripIndex, tailof(7), headof(7)},
        {GreenStripIndex, headof(5), tailof(5)},
        {BlueStripIndex, tailof(10), headof(10)},
        {GreenStripIndex, tailof(2), headof(2)},

        {RedStripIndex, headof(5), tailof(5)},
        {BlackStripIndex, tailof(4), headof(4)},
        {BlackStripIndex, headof(5), tailof(5)},
        {BlueStripIndex, headof(5), tailof(5)},
        {BlueStripIndex, tailof(4), headof(4)},
        {GreenStripIndex, tailof(1), headof(1)},

        {BlackStripIndex, tailof(9), headof(9)},
        {BlueStripIndex, headof(6), tailof(6)},
        {GreenStripIndex, tailof(6), headof(6)},
        {BlueStripIndex, tailof(9), headof(9)},

        {BlackStripIndex, tailof(3), headof(3)},
        {BlackStripIndex, tailof(8), headof(8)},
        {BlackStripIndex, headof(6), tailof(6)},
        {BlueStripIndex, tailof(8), headof(8)},
        {BlueStripIndex, tailof(3), headof(3)},

        {BlackStripIndex, tailof(2), headof(2)},
        {BlackStripIndex, headof(7), tailof(7)},
        {BlueStripIndex, headof(7), tailof(7)},
        {BlueStripIndex, tailof(2), headof(2)},

        {BlackStripIndex, tailof(1), headof(1)},
        {BlueStripIndex, tailof(1), headof(1)}
    };
}

#endif