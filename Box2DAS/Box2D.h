#include <stdlib.h>
#include <stdio.h>
#include "AS3.h"
#include "Box2D/Box2D.h";
#include "Box2D/Collision/b2BroadPhase.cpp";
#include "Box2D/Collision/b2CollideCircle.cpp";
#include "Box2D/Collision/b2CollidePolygon.cpp";
#include "Box2D/Collision/b2Collision.cpp";
#include "Box2D/Collision/b2Distance.cpp";
#include "Box2D/Collision/b2DynamicTree.cpp";
#include "Box2D/Collision/b2TimeOfImpact.cpp";
#include "Box2D/Collision/Shapes/b2CircleShape.cpp";
#include "Box2D/Collision/Shapes/b2PolygonShape.cpp";
#include "Box2D/Common/b2BlockAllocator.cpp";
#include "Box2D/Common/b2Math.cpp";
#include "Box2D/Common/b2Settings.cpp";
#include "Box2D/Common/b2StackAllocator.cpp";
#include "Box2D/Dynamics/b2Body.cpp";
#include "Box2D/Dynamics/b2ContactManager.cpp";
#include "Box2D/Dynamics/b2Fixture.cpp";
#include "Box2D/Dynamics/b2Island.cpp";
#include "Box2D/Dynamics/b2World.cpp";
#include "Box2D/Dynamics/b2WorldCallbacks.cpp";
#include "Box2D/Dynamics/Contacts/b2CircleContact.cpp";
#include "Box2D/Dynamics/Contacts/b2Contact.cpp";
#include "Box2D/Dynamics/Contacts/b2ContactSolver.cpp";
#include "Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp";
#include "Box2D/Dynamics/Contacts/b2PolygonContact.cpp";
#include "Box2D/Dynamics/Joints/b2DistanceJoint.cpp";
#include "Box2D/Dynamics/Joints/b2GearJoint.cpp";
#include "Box2D/Dynamics/Joints/b2Joint.cpp";
#include "Box2D/Dynamics/Joints/b2LineJoint.cpp";
#include "Box2D/Dynamics/Joints/b2MouseJoint.cpp";
#include "Box2D/Dynamics/Joints/b2PrismaticJoint.cpp";
#include "Box2D/Dynamics/Joints/b2PulleyJoint.cpp";
#include "Box2D/Dynamics/Joints/b2RevoluteJoint.cpp";
#include "Box2D/Dynamics/Joints/b2FrictionJoint.cpp";
#include "Box2D/Dynamics/Joints/b2WeldJoint.cpp";
#include "Box2D/ConvexDecomposition/b2Triangle.cpp";

#include "Box2D/ConvexDecomposition/b2Polygon.cpp";


/// Easy tracing.

AS3_Val as3_ts;
#define TS(string) as3_ts = AS3_String(string); AS3_Trace(as3_ts); AS3_Release(as3_ts);
#define T(s); TS(#s);


/// Macro to export a C++ function with NULL state data.

#define AS3F(name) AS3_Function(NULL, name)

/// Macro that allows us to return a pointer to flash without worrying about releasing it.

AS3_Val as3_ptr;
#define return_as3_ptr(ptr) AS3_Release(as3_ptr); as3_ptr = AS3_Ptr(ptr); return as3_ptr;


/// Some macros for easily exporting new / delete functions to AS3.

#define as3_new(type) AS3_Val type##_new(void* data, AS3_Val args) { return_as3_ptr(new type()); };
#define as3_del(type) AS3_Val type##_delete(void* data, AS3_Val args) { type* p; AS3_ArrayValue(args, "PtrType", &p); delete p; return AS3_Null(); };
#define as3_new_del(type) as3_new(type); as3_del(type);


/// Build simple constructors / destructors for all the ...def objects

as3_new_del(b2BodyDef);
as3_new_del(b2CircleShape);
as3_new_del(b2PolygonShape);
as3_new_del(b2FixtureDef);
as3_new_del(b2DistanceJointDef);
as3_new_del(b2GearJointDef);
as3_new_del(b2LineJointDef);
as3_new_del(b2MouseJointDef);
as3_new_del(b2PrismaticJointDef);
as3_new_del(b2PulleyJointDef);
as3_new_del(b2RevoluteJointDef);
as3_new_del(b2FrictionJointDef);
as3_new_del(b2WeldJointDef);
as3_new_del(b2MassData);

/// And for b2Distance stuff

as3_new_del(b2DistanceInput);
as3_new_del(b2DistanceOutput);
as3_new_del(b2SimplexCache);

/// AS3ValType's value tracker. This can be used to get AS3 stuff hanging out in C++ land.

asm("public var vt:ValueTracker = CTypemap.AS3ValType.valueTracker;");

/// An AS3 Array that can be manipulated via asm.

asm("public var arr:Array;");