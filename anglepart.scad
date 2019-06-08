/**
 * Simple triangles library
 *
 * Authors:
 *   - Eero 'rambo' af Heurlin 2010-
 *
 * License: LGPL 2.1
 */


/**
 * Standard right-angled triangle
 *
 * @param number  o_len  Length of the opposite side
 * @param number  a_len  Length of the adjacent side
 * @param number  depth  How wide/deep the triangle is in the 3rd dimension
 * @param boolean center Whether to center the triangle on the origin
 * @todo a better way ?
 */
module triangle(o_len, a_len, depth, center=false)
{
    centroid = center ? [-a_len/3, -o_len/3, -depth/2] : [0, 0, 0];
    translate(centroid) linear_extrude(height=depth)
    {
        polygon(points=[[0,0],[a_len,0],[0,o_len]], paths=[[0,1,2]]);
    }
}

/**
 * Standard right-angled triangle (tangent version)
 *
 * @param number  tan_angle Angle of adjacent to hypotenuse (ie tangent)
 * @param number  a_len     Length of the adjacent side
 * @param number  depth     How wide/deep the triangle is in the 3rd dimension
 * @param boolean center    Whether to center the triangle on the origin
 */
module a_triangle(tan_angle, a_len, depth, center=false)
{
    triangle(tan(tan_angle) * a_len, a_len, depth, center);
}

mirror(v=[1,0,0]){
       difference() {

triangle(22.5,76.2,25.4);
for (i = [-2:2])
    for (j = [-2:2])
        rotate([-90,0,-22.5])
            translate ([19.05,-12.5, 0]) //moves the holes 
                cylinder (d = 1.7, h = 50, center = true);
}
}

mirror(v=[0,0,0]){
       difference() {

triangle(22.5,76.2,25.4);
for (i = [-2:2])
    for (j = [-2:2])
        rotate([-90,0,-22.5])
            translate ([19.05,-12.5, 0]) //moves the holes 
                cylinder (d = 1.7, h = 50, center = true);
}
}
