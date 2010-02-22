/*

To compile the SWC:

alc-on; g++ Box2D.c -I.. -O3 -Wall -swc -DOSX -o Box2D.swc; alc-off

To see the temporary AS files:

export ACHACKS_TMPS=1


*/
#include "Box2DAS/Box2D.h";

class WorldListener : public b2ContactListener, public b2DestructionListener {
public:
	
	AS3_Val usr;

	void ReportContact(b2Contact* c, char* phase) {
		AS3_CallTS(phase, usr, "PtrType, AS3ValType, AS3ValType", c, 
			(AS3_Val)c->m_fixtureA->m_userData,
			(AS3_Val)c->m_fixtureB->m_userData);
	}
	
	void BeginContact(b2Contact* contact) {
		if(contact->m_fixtureA->m_reportBeginContact || contact->m_fixtureB->m_reportBeginContact) {
			ReportContact(contact, "BeginContact");
		}
	}
	
	void EndContact(b2Contact* contact) {
		if(contact->m_fixtureA->m_reportEndContact || contact->m_fixtureB->m_reportEndContact) {
			ReportContact(contact, "EndContact");
		}
	}
	
	void PreSolve(b2Contact* c, const b2Manifold* o) {
		/// Dont bother with zero-point pre-solve events. Can't see the point in reporting these.
		if(c->IsTouching()) {
			if(c->m_fixtureA->m_reportPreSolve || c->m_fixtureB->m_reportPreSolve) {
				AS3_CallTS("PreSolve", usr, "PtrType, AS3ValType, AS3ValType, PtrType", c, 
					(AS3_Val)c->m_fixtureA->m_userData,
					(AS3_Val)c->m_fixtureB->m_userData, o);
			}
		}
	}
	
	void PostSolve(b2Contact* c, const b2ContactImpulse* i) {
		if(c->m_fixtureA->m_reportPostSolve || c->m_fixtureB->m_reportPostSolve) {
			AS3_CallTS("PostSolve", usr, "PtrType, AS3ValType, AS3ValType, PtrType", c, 
				(AS3_Val)c->m_fixtureA->m_userData,
				(AS3_Val)c->m_fixtureB->m_userData, i);
		}
	}
	
	void SayGoodbye(b2Joint* j) {
		AS3_Release((AS3_Val)j->m_userData);
	}
	
	void SayGoodbye(b2Fixture* f) {
		AS3_Release((AS3_Val)f->m_userData);
	}
};

class QueryCallback : public b2QueryCallback {
public:
	
	AS3_Val callback;
	
	bool ReportFixture(b2Fixture* fixture) {
		return AS3_IntValue(AS3_CallT(
			callback, NULL, 
			"AS3ValType", 
			(AS3_Val)fixture->m_userData
		)) == 1;
	}
};

class RayCastCallback : public b2RayCastCallback {
public: 
	
	AS3_Val callback;
	
	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
		return AS3_NumberValue(AS3_CallT(
			callback, NULL, 
			"AS3ValType, DoubleType, DoubleType, DoubleType, DoubleType, DoubleType",
			(AS3_Val)fixture->m_userData, point.x, point.y, normal.x, normal.y, fraction));
	}
};

AS3_Val b2World_QueryAABB(void* data, AS3_Val args) {
	b2World* w;
	double x1, x2, y1, y2;
	AS3_Val f;
	QueryCallback cb;
	b2AABB aabb;
	AS3_ArrayValue(args, "PtrType, AS3ValType, DoubleType, DoubleType, DoubleType, DoubleType", 
		&w, &f, &x1, &y1, &x2, &y2);
	aabb.lowerBound.x = x1;
	aabb.lowerBound.y = y1;
	aabb.upperBound.x = x2;
	aabb.upperBound.y = y2;
	cb.callback = f;
	w->QueryAABB(&cb, aabb);
	AS3_Release(f);
	return AS3_Null();
}

AS3_Val b2World_RayCast(void* data, AS3_Val args) {
	b2World* w;
	double x1, x2, y1, y2;
	AS3_Val f;
	RayCastCallback cb;
	AS3_ArrayValue(args, "PtrType, AS3ValType, DoubleType, DoubleType, DoubleType, DoubleType", 
		&w, &f, &x1, &y1, &x2, &y2);
	b2Vec2 p1(x1, y1);
	b2Vec2 p2(x2, y2);
	cb.callback = f;
	w->RayCast(&cb, p1, p2);
	AS3_Release(f);
	return AS3_Null();
}

AS3_Val b2World_new(void* data, AS3_Val args) {
	int s;
	b2Vec2 g;
	double gx, gy;
	AS3_Val usr;
	AS3_ArrayValue(args, "AS3ValType, DoubleType, DoubleType, IntType", &usr, &gx, &gy, &s);
	g.Set(gx, gy);
	b2World* w = new b2World(g, s == 1);
	WorldListener* l = new WorldListener();
	w->SetContactListener(l);
	w->SetDestructionListener(l);
	l->usr = usr;
	return_as3_ptr(w);
}

AS3_Val b2World_delete(void* data, AS3_Val args) {
	b2World* w;
	AS3_ArrayValue(args, "PtrType", &w);
	for(b2Body* b = w->m_bodyList; b; b = b->m_next) {
		for(b2Fixture* f = b->m_fixtureList; f; f = f->m_next) {
			AS3_Release((AS3_Val)f->m_userData);
		}
		AS3_Release((AS3_Val)b->m_userData);
	}
	for(b2Joint* j = w->m_jointList; j; j = j->m_next) {
		AS3_Release((AS3_Val)j->m_userData);
	}
	WorldListener* l = (WorldListener*)w->m_destructionListener;
	AS3_Release(l->usr);
	delete l;
	delete w;
	return AS3_Null();
}

AS3_Val b2World_Step(void* data, AS3_Val args) {
	b2World* w;
	double ts;
	int vi, pi;
	AS3_ArrayValue(args, "PtrType, DoubleType, IntType, IntType", &w, &ts, &vi, &pi);	
	w->Step(ts, vi, pi);
	return AS3_Null();
}

AS3_Val b2World_CreateBody(void* data, AS3_Val args) {
	AS3_Val usr;
	b2World* w;
	b2BodyDef* d;
	AS3_ArrayValue(args, "AS3ValType, PtrType, PtrType", &usr, &w, &d);
	b2Body* b = w->CreateBody(d);
	b->m_userData = usr;
	return_as3_ptr(b);
}

AS3_Val b2World_DestroyBody(void* data, AS3_Val args) {
	b2World* w;
	b2Body* b;
	AS3_ArrayValue(args, "PtrType, PtrType", &w, &b);
	AS3_Release((AS3_Val)b->m_userData);
	w->DestroyBody(b);
	return AS3_Null();
}

AS3_Val b2World_CreateJoint(void* data, AS3_Val args) {
	AS3_Val usr;
	b2World* w;
	b2JointDef* d;
	AS3_ArrayValue(args, "AS3ValType, PtrType, PtrType", &usr, &w, &d);
	b2Joint* j = w->CreateJoint(d);
	j->m_userData = usr;
	return_as3_ptr(j);
}

AS3_Val b2World_DestroyJoint(void* data, AS3_Val args) {
	b2World* w;
	b2Joint* j;
	AS3_ArrayValue(args, "PtrType, PtrType", &w, &j);
	AS3_Release((AS3_Val)j->m_userData);
	w->DestroyJoint(j);
	return AS3_Null();
}

AS3_Val b2Body_CreateFixture(void* data, AS3_Val args) {
	AS3_Val usr;
	b2Body* b;
	b2FixtureDef* d;
	AS3_ArrayValue(args, "AS3ValType, PtrType, PtrType", &usr, &b, &d);
	b2Fixture* f = b->CreateFixture(d);
	f->m_userData = usr;
	return_as3_ptr(f);
}

AS3_Val b2Body_DestroyFixture(void* data, AS3_Val args) {
	b2Body* b;
	b2Fixture* f;
	AS3_ArrayValue(args, "PtrType, PtrType", &b, &f);
	AS3_Release((AS3_Val)f->m_userData);
	b->DestroyFixture(f);
	return AS3_Null();
}

AS3_Val b2Body_SetTransform(void* data, AS3_Val args) {
	b2Body* b;
	double x, y, a;
	AS3_ArrayValue(args, "PtrType, DoubleType, DoubleType, DoubleType", &b, &x, &y, &a);
	b2Vec2 v;
	v.Set(x, y);
	b->SetTransform(v, a);
	return AS3_Null();
}

AS3_Val b2Body_ResetMassData(void* data, AS3_Val args) {
	b2Body* b;
	AS3_ArrayValue(args, "PtrType", &b);
	b->ResetMassData();
	return AS3_Null();
}

AS3_Val b2Body_SetMassData(void* data, AS3_Val args) {
	b2Body* b;
	b2MassData* m;
	AS3_ArrayValue(args, "PtrType, PtrType", &b, &m);
	b->SetMassData(m);
	return AS3_Null();
}

AS3_Val b2Body_GetMassData(void* data, AS3_Val args) {
	b2Body* b;
	b2MassData* m;
	AS3_ArrayValue(args, "PtrType, PtrType", &b, &m);
	b->GetMassData(m);
	return AS3_Null();
}

AS3_Val b2Body_SetActive(void* data, AS3_Val args) {
	b2Body* b;
	int a;
	AS3_ArrayValue(args, "PtrType, IntType", &b, &a);
	b->SetActive(a == 1);
	return AS3_Null();
}

AS3_Val b2Body_SetType(void* data, AS3_Val args) {
	b2Body* b;
	b2BodyType t;
	AS3_ArrayValue(args, "PtrType, IntType", &b, &t);
	b->SetType(t);
	return AS3_Null();
}

AS3_Val b2Contact_Update(void* data, AS3_Val args) {
	b2Contact* c;
	AS3_ArrayValue(args, "PtrType", &c);
	c->Update(c->m_fixtureA->m_body->m_world->m_contactManager.m_contactListener);
	return AS3_Null();
}

AS3_Val b2PolygonShape_Decompose(void* data, AS3_Val args) {
	b2Polygon p;
	b2Polygon results[100];
	asm("%0 vt.get(%1)[0].length / 2;" : "=r" (p.nVertices) : "r" (args));	
	if(p.nVertices <= 2) {
		return AS3_Null();
	}
	int i, j;
	p.x = new float32[p.nVertices];
	p.y = new float32[p.nVertices];
	asm("var v:Vector.<Number> = vt.get(%0)[0];" : : "r" (args));	
	for(i = 0; i < p.nVertices; ++i) {
		asm("%0 v[%1 * 2]" : "=r" (p.x[i]) : "r" (i));
		asm("%0 v[%1 * 2 + 1]" : "=r" (p.y[i]) : "r" (i));
	}
	TraceEdge(&p);
	int r = DecomposeConvex(&p, results, 100); // Arbitrary maximum
	asm("arr = [];");
	b2PolygonShape* s;
	for(i = 0; i < r; ++i) {
		s = new b2PolygonShape();
		asm("arr.push(%0)" : : "r" (s));
		for(j = 0; j < results[i].nVertices; ++j) {
			s->m_vertices[j].x = results[i].x[j];
			s->m_vertices[j].y = results[i].y[j];
		}
		s->Set(s->m_vertices, results[i].nVertices);
	}
	return AS3_Null();
}

AS3_Val b2Distance(void* data, AS3_Val args) {
	b2DistanceInput *i;
	b2DistanceOutput *o;
	b2SimplexCache *c;
	AS3_ArrayValue(args, "PtrType, PtrType, PtrType", &o, &c, &i);
	b2Distance(o, c, i);
	return AS3_Null();
}


int main() {
		
	AS3_LibInit(AS3_Object(
	
		"b2World_new:AS3ValType,"
		"b2World_Step:AS3ValType,"
		"b2World_CreateBody:AS3ValType,"
		"b2World_DestroyBody:AS3ValType,"
		"b2World_CreateJoint:AS3ValType,"
		"b2World_DestroyJoint:AS3ValType,"
		"b2World_delete:AS3ValType,"
		"b2World_QueryAABB:AS3ValType,"
		"b2World_RayCast:AS3ValType,"

		"b2Body_CreateFixture:AS3ValType,"
		"b2Body_DestroyFixture:AS3ValType,"
		"b2Body_SetTransform:AS3ValType,"
		"b2Body_ResetMassData:AS3ValType,"
		"b2Body_GetMassData:AS3ValType,"
		"b2Body_SetMassData:AS3ValType,"
		"b2Body_SetActive:AS3ValType,"
		"b2Body_SetType:AS3ValType,"

		"b2BodyDef_new:AS3ValType,"
		"b2BodyDef_delete:AS3ValType,"

		"b2CircleShape_new:AS3ValType,"
		"b2CircleShape_delete:AS3ValType,"

		"b2PolygonShape_new:AS3ValType,"
		"b2PolygonShape_delete:AS3ValType,"

		"b2FixtureDef_new:AS3ValType,"
		"b2FixtureDef_delete:AS3ValType,"

		"b2DistanceJointDef_new:AS3ValType,"
		"b2DistanceJointDef_delete:AS3ValType,"

		"b2GearJointDef_new:AS3ValType,"
		"b2GearJointDef_delete:AS3ValType,"

		"b2LineJointDef_new:AS3ValType,"
		"b2LineJointDef_delete:AS3ValType,"

		"b2MouseJointDef_new:AS3ValType,"
		"b2MouseJointDef_delete:AS3ValType,"

		"b2PrismaticJointDef_new:AS3ValType,"
		"b2PrismaticJointDef_delete:AS3ValType,"

		"b2PulleyJointDef_new:AS3ValType,"
		"b2PulleyJointDef_delete:AS3ValType,"

		"b2RevoluteJointDef_new:AS3ValType,"
		"b2RevoluteJointDef_delete:AS3ValType,"
		
		"b2WeldJointDef_new:AS3ValType,"
		"b2WeldJointDef_delete:AS3ValType,"
		
		"b2FrictionJointDef_new:AS3ValType,"
		"b2FrictionJointDef_delete:AS3ValType,"
		
		"b2MassData_new:AS3ValType,"
		"b2MassData_delete:AS3ValType,"
		
		"b2Contact_Update:AS3ValType,"
		
		"b2PolygonShape_Decompose:AS3ValType,"
		
		"b2DistanceInput_new:AS3ValType,"
		"b2DistanceInput_delete:AS3ValType,"
		
		"b2DistanceOutput_new:AS3ValType,"
		"b2DistanceOutput_delete:AS3ValType,"
		
		"b2SimplexCache_new:AS3ValType,"
		"b2SimplexCache_delete:AS3ValType,"
		
		"b2Distance:AS3ValType",
		
		AS3F(b2World_new),
		AS3F(b2World_Step),
		AS3F(b2World_CreateBody),
		AS3F(b2World_DestroyBody),
		AS3F(b2World_CreateJoint),
		AS3F(b2World_DestroyJoint),
		AS3F(b2World_delete),
		AS3F(b2World_QueryAABB),
		AS3F(b2World_RayCast),
		
		AS3F(b2Body_CreateFixture),
		AS3F(b2Body_DestroyFixture),
		AS3F(b2Body_SetTransform),
		AS3F(b2Body_ResetMassData),
		AS3F(b2Body_GetMassData),
		AS3F(b2Body_SetMassData),
		AS3F(b2Body_SetActive),
		AS3F(b2Body_SetType),
		
		AS3F(b2BodyDef_new),
		AS3F(b2BodyDef_delete),
		
		AS3F(b2CircleShape_new),
		AS3F(b2CircleShape_delete),
		
		AS3F(b2PolygonShape_new),
		AS3F(b2PolygonShape_delete),
		
		AS3F(b2FixtureDef_new),
		AS3F(b2FixtureDef_delete),
		
		AS3F(b2DistanceJointDef_new),
		AS3F(b2DistanceJointDef_delete),
		
		AS3F(b2GearJointDef_new),
		AS3F(b2GearJointDef_delete),
		
		AS3F(b2LineJointDef_new),
		AS3F(b2LineJointDef_delete),
		
		AS3F(b2MouseJointDef_new),
		AS3F(b2MouseJointDef_delete),
		
		AS3F(b2PrismaticJointDef_new),
		AS3F(b2PrismaticJointDef_delete),
		
		AS3F(b2PulleyJointDef_new),
		AS3F(b2PulleyJointDef_delete),
		
		AS3F(b2RevoluteJointDef_new),
		AS3F(b2RevoluteJointDef_delete),
		
		AS3F(b2WeldJointDef_new),
		AS3F(b2WeldJointDef_delete),
		
		AS3F(b2FrictionJointDef_new),
		AS3F(b2FrictionJointDef_delete),
		
		AS3F(b2MassData_new),
		AS3F(b2MassData_delete),
		
		AS3F(b2Contact_Update),
		
		AS3F(b2PolygonShape_Decompose),
		
		AS3F(b2DistanceInput_new),
		AS3F(b2DistanceInput_delete),
		
		AS3F(b2DistanceOutput_new),
		AS3F(b2DistanceOutput_delete),
		
		AS3F(b2SimplexCache_new),
		AS3F(b2SimplexCache_delete),
		
		AS3F(b2Distance)
		
	));
	return 0; 
}