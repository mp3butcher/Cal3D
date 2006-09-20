/* (C) 2004-2006 Mekensleep
 *
 *	Mekensleep
 *	24 rue vieille du temple
 *	75004 Paris
 *       licensing@mekensleep.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Authors:
 *  Igor Kravtchenko <igor@tsarevitch.org>
 *
 */

#include <cal3d/quaternion.h>

bool testCompressAndDecompressQuaternionAndCheckIfEquivalent(float qx, float qy, float qz, float qw)
{
	CalQuaternion q;

	float qn = 1.0f / sqrt(qx*qx + qy*qy + qz*qz + qw*qw);
	qx *= qn;
	qy *= qn;
	qz *= qn;
	qw *= qn;

	q.x = qx;
	q.y = qy;
	q.z = qz;
	q.w = qw;
	short c0, c1, c2;
	q.compress(c0, c1, c2);
	q.decompress(c0, c1, c2);

	float difx = fabs(qx) - fabs(q.x);
	float dify = fabs(qy) - fabs(q.y);
	float difz = fabs(qz) - fabs(q.z);
	float difw = fabs(qw) - fabs(q.w);

	if (fabs(difx) > 1e-2 || fabs(dify) > 1e-2 || fabs(dify) > 1e-2 || fabs(difz) > 1e-2)
		return false;

	return true;
}

int main(int argc, char *argv[])
{
	// test all of the 16 possible signs a quaternion can take
	for (int i = 0; i < 16; i++) {
		float qx = -1;
		float qy = -1;
		float qz = -1;
		float qw = -1;

		if (i & 1)
			qw = 1;

		if (i & 2)
			qz = 1;

		if (i & 4)
			qy = 1;

		if (i & 8)
			qx = 1;

		printf("Testing compression/decompression with quaternion %.2f, %.2f, %.2f, %.2f\n", qx, qy, qz, qw);
		bool res = testCompressAndDecompressQuaternionAndCheckIfEquivalent(qx, qy, qy, qx);
		if (res == false) {
			printf("Failed.\n");
			return -1;
		}
		printf("Success!\n");
	}

	printf("All tests succeeded.\n");
	return 0;
}
