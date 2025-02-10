#pragma once

enum keycode
{
	a = 0x1e, b = 0x30, c = 0x2e, d = 0x20, e = 0x12, f = 0x21, g = 0x22,
	h = 0x23, i = 0x17, j = 0x24, k = 0x25, l = 0x26, m = 0x32, n = 0x31,
	o = 0x18, p = 0x19, q = 0x10, r = 0x13, s = 0x1f, t = 0x14, u = 0x16,
	v = 0x2f, w = 0x11, x = 0x2d, y = 0x15, z = 0x2c, esc = 0x01,

	zero = 0x0b, one = 0x02,	/* values 2-9 follow 1 */

	dash = 0x0c, equal = 0x0d, space = 0x39, del = 0x0e, enter = 0x1c,

	f1 = 0x3b, f2 = 0x3c, f3 = 0x3d, f4 = 0x3e, f5 = 0x3f, f6 = 0x40, f7 = 0x41,
	f8 = 0x42,
	extended = 0xe0,
};

enum keycode_ext
{
	up = 0x48, down = 0x50, left = 0x4b, right = 0x4d,
};
