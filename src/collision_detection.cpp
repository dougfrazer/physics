
#include "vector3.h"
#include "matrix.h"
#include "geometry.h"
#include "physics.h"
#include <limits>
#include <vector>

static const vector3 origin;


//******************************************************************************
// Support:
// --------
//   This will attempt to find the closest point in the geometry to the vector
//   passed in.
//******************************************************************************;
static vector3 Support(const vector3 d, const GEOMETRY* Geo, const matrix4* m);
static vector3 Support(const vector3 d, const GEOMETRY* A, const matrix4* at, const GEOMETRY* B, const matrix4* bt);

//******************************************************************************
// Simplex:
// --------
//   This function will take in a list of points, and a direction.  It knows the last
//   point was a point that was just added.  We will test to see if that point is part of
//   a tetrahedron surrounding the origin.  If it is not, we discard it, and change the direction to face
//   the origin.  If it is part of the tetrahedron, we leave it, and update our search direction
//   towards the origin but in the direction perpendicular to what we just added.
//
// Note: this function will assume the last point in the list be the point we just added
// Note: this function modifies both the list and the direction.
//******************************************************************************
static bool Simplex(std::vector<vector3>* list, vector3* d, const vector3 dest);


// todo hack: just assuming the one board at 0,0,0 is the only plane.
static     vector3 s_planeNormal = { 0.0f, 1.0f, 0.0f };
static     vector3 s_planePoint = { 0.0f, 0.0f, 0.0f };

//******************************************************************************
static vector3 GetCollisionPoint(const PHYSICS* a, const PHYSICS* b, float* depth)
{
    matrix4 m;
    m.translate(a->m_dynamic.m_position);
    m.rotate(a->m_dynamic.m_rotation);

    float min = std::numeric_limits<float>::infinity();
    vector3 ret;
    for (int i = 0; i < a->m_geometry->NumVertices; i++)
    {
        vector3 current = vector3(m * vector4(a->m_geometry->VertexList[i]));
        float d = current.dot(s_planeNormal);
        if (d < min)
        {
            min = d;
            ret = current;
        }
    }
    assert(min != std::numeric_limits<float>::infinity());
    if (depth)
    {
        *depth = fabs(min);
    }
    return ret;
}
//******************************************************************************
bool DetectCollision(const PHYSICS* a, const PHYSICS* b, CollisionData* outCollision)
{
    std::vector<vector3> simplex;

    // TODO: broad phase

    // start with any point in the mikowski difference
    vector3 start((vector3(a->m_geometry->VertexList[0]) + vector3(a->m_dynamic.m_position)) -
                  (vector3(b->m_geometry->VertexList[0]) + vector3(b->m_dynamic.m_position)));
    simplex.push_back(start);

    // go towards the origin
    vector3 d = -start;
    while (true)
    {
        matrix4 a_transform;
        a_transform.translate(a->m_dynamic.m_position);
        a_transform.rotate(a->m_dynamic.m_rotation);
        matrix4 b_transform;
        b_transform.translate(b->m_dynamic.m_position);
        b_transform.rotate(b->m_dynamic.m_rotation);
        vector3 support = Support(d, a->m_geometry, &a_transform, b->m_geometry, &b_transform);
        if (d.dot(support) < 0)
        {
            return false;
        }
        simplex.push_back(support);
        if (Simplex(&simplex, &d, origin))
        {
            if (outCollision)
            {
                outCollision->planeNormal = s_planeNormal;
                outCollision->planePoint = s_planePoint;
                float collisionDepth = 0.0f;
                outCollision->point = GetCollisionPoint(a,b,&collisionDepth);
                outCollision->depth = collisionDepth;
            }
            return true;
        }
    }

    return false;
}

//******************************************************************************
// Find the furthest point in the minkowski difference of A and B in the direction d.
// This is the same as saying find the furthest point in A in the direction of d
// MINUS the furthest point in B in the opposite direction
//******************************************************************************
vector3 Support(const vector3 d, const GEOMETRY* A, const matrix4* A_transform, const GEOMETRY* B, const matrix4* B_transform)
{
    return Support(d, A, A_transform) - Support(-d, B, B_transform);
}
//******************************************************************************
vector3 Support(const vector3 d, const GEOMETRY* Geo, const matrix4* const m)
{
    float dot_product = -std::numeric_limits<float>::infinity();
    vector3 closest;
    for (int i = 0; i < Geo->NumVertices; i++)
    {
        vector3 t = vector3(*m * vector4(Geo->VertexList[i]));

        float c = d.dot(t);
        if (c >= dot_product)
        {
            dot_product = c;
            closest = t;
        }
    }
    assert(dot_product != -std::numeric_limits<float>::infinity());
    return closest;
}
//******************************************************************************

//******************************************************************************
bool Simplex(std::vector<vector3>* simplex, vector3* dir, const vector3 dest)
{
    if (simplex->size() == 2)
    {
        vector3 a = simplex->at(1);
        vector3 b = simplex->at(0);
        vector3 ab = b - a;
        vector3 ao = dest - a;
        if (ab.dot(ao) > 0)
        {
            *dir = ab.cross(ao).cross(ab);
        }
        else
        {
            *dir = ao;
            simplex->pop_back();
        }
    }
    else if (simplex->size() == 3)
    {
        // we have a triangle/plane:
        vector3 a = simplex->at(2);
        vector3 b = simplex->at(1);
        vector3 c = simplex->at(0);
        simplex->clear();
        vector3 ab = b - a;
        vector3 ac = c - a;
        vector3 abc = ab.cross(ac);
        vector3 ao = dest - a;

        // first check to see if the point is above AC
        if (abc.cross(ac).dot(ao) > 0)
        {
            if (ac.dot(ao) > 0)
            {
                *dir = ac.cross(ao).cross(ac);
                simplex->push_back(a);
                simplex->push_back(c);
            }
            else if (ab.dot(ao))
            {
                *dir = ab.cross(ao).cross(ab);
                simplex->push_back(a);
                simplex->push_back(b);
            }
            else
            {
                *dir = ao;
                simplex->push_back(a);
            }
            // then check to see if the point is below AB
        }
        else if (ab.cross(abc).dot(ao) > 0)
        {
            if (ab.dot(ao) > 0)
            {
                *dir = ab.cross(ao).cross(ab);
                simplex->push_back(a);
                simplex->push_back(b);
            }
            else if (ab.dot(ao))
            {
                *dir = ac.cross(ao).cross(ac);
                simplex->push_back(a);
                simplex->push_back(c);
            }
            else
            {
                *dir = ao;
                simplex->push_back(a);
            }
            // OK, the point is necessarily below AC and above AB (so it is within the triangle),
            // just need to figure out which direction to search
        }
        else if (abc.dot(ao) > 0)
        {
            *dir = abc;
            simplex->push_back(a);
            simplex->push_back(b);
            simplex->push_back(c);
        }
        else
        {
            *dir = -abc;
            simplex->push_back(a);
            simplex->push_back(c);
            simplex->push_back(b);
        }
    }
    else if (simplex->size() == 4)
    {
        // we have a tetrahedron
        // return whether or not the origin is enclosed in the tetrahedron
        vector3 a = simplex->at(3);
        vector3 b = simplex->at(2);
        vector3 c = simplex->at(1);
        vector3 d = simplex->at(0);
        simplex->clear();
        vector3 ab = b - a;
        vector3 ac = c - a;
        vector3 ad = d - a;
        vector3 ao = dest - a;
        vector3 abd = ab.cross(ad);
        vector3 adc = ad.cross(ac);
        vector3 acb = ac.cross(ab);
        if (abd.dot(ao) > 0)
        {
            *dir = abd;
            simplex->push_back(a);
            simplex->push_back(b);
            simplex->push_back(d);
        }
        else if (adc.dot(ao) > 0)
        {
            *dir = adc;
            simplex->push_back(a);
            simplex->push_back(d);
            simplex->push_back(c);
        }
        else if (acb.dot(ao) > 0)
        {
            *dir = acb;
            simplex->push_back(a);
            simplex->push_back(c);
            simplex->push_back(b);
        }
        else
        {
            return true;
        }
    }
    else
    {
        assert(false);  // should not be possible
    }
    return false;
}
//******************************************************************************
