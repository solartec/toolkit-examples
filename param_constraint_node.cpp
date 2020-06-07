//
// Created by redcat1
//

#include "param_constraint_node.h"
#include <param_geom_node.h>
#include <gce_api.h>

//----------------------------------------------------------------------------------------
//
// ---
AppConstraintNode::AppConstraintNode()
  : conItem(GCE_NULL_C)
{}

//----------------------------------------------------------------------------------------
//
// ---
bool AppConstraintNode::Remove(GCE_system system)
{
  const bool res = GCE_RemoveConstraint(system, ConItem());
  if (res)
  {
    conItem = GCE_NULL_C;
  }
  return res;
}

//----------------------------------------------------------------------------------------
//
// ---
AppFixLengthNode::AppFixLengthNode(AppGeomNodeCRef gNode)
 : AppConstraintNode()
 , geomItem(gNode.GceItem())
{}

//----------------------------------------------------------------------------------------
//
// ---
constraint_item AppFixLengthNode::Formulate(GCE_system system)
{
  return GCE_FixRadius(system, geomItem);
}

//----------------------------------------------------------------------------------------
//
// ---
AppFixRadiusNode::AppFixRadiusNode(AppGeomNodeCRef gNode)
  : AppConstraintNode()
  , circleItem(gNode.GceItem())
{}

//----------------------------------------------------------------------------------------
//
// ---
constraint_item AppFixRadiusNode::Formulate(GCE_system system)
{
  return GCE_FixRadius(system, circleItem);
}

//----------------------------------------------------------------------------------------
//
// ---
AppFixGeomNode::AppFixGeomNode(AppGeomNodeCRef gNode)
  : AppConstraintNode()
  , geomItem(gNode.GceItem())
{}

//----------------------------------------------------------------------------------------
//
// ---
geom_item AppFixGeomNode::Formulate(GCE_system solver)
{
  return GCE_FixGeom(solver, geomItem);
}

//----------------------------------------------------------------------------------------
//
// ---
AppAlignNode::AppAlignNode(AppGeomNodeCRef geom, constraint_type cType)
  : AppConstraintNode()
  , conType(cType)
  , geomItem(geom.GceItem())
{}

//----------------------------------------------------------------------------------------
//
// ---
constraint_item AppAlignNode::Formulate(GCE_system solver)
{
  return GCE_AddUnaryConstraint(solver, conType, geomItem);
}

//----------------------------------------------------------------------------------------
//
// ---
AppDistanceNode::AppDistanceNode(AppGeomNodeCRef geom1, AppGeomNodeCRef geom2, double dVal)
  : AppConstraintNode()
  , gItem1(geom1.GceItem())
  , gItem2(geom2.GceItem())
  , dimPars()
{
  dimPars.dPars.dimValue = dVal;
}

//----------------------------------------------------------------------------------------
//
// ---
constraint_item AppDistanceNode::Formulate(GCE_system solver)
{
  geom_item g[2] = { gItem1, gItem2 };
  return GCE_AddDistance(solver, g, dimPars);
}

//----------------------------------------------------------------------------------------
//
// ---
GCE_result AppDistanceNode::ChangeDimension(GCE_system solver, double newDimVal)
{
  auto result = GCE_ChangeDrivingDimension(solver, conItem, newDimVal);
  if (result == GCE_RESULT_Ok)
  {
    dimPars.dPars.dimValue = newDimVal;
  }
  return result;
}

//----------------------------------------------------------------------------------------
//
// ---
AppConnectSegmentsNode::AppConnectSegmentsNode(AppGeomNodeCRef gNode1, AppGeomNodeCRef gNode2)
  : AppConstraintNode()
  , crvItem1(gNode1.GceItem())
  , crvItem2(gNode1.GceItem())
{}

//----------------------------------------------------------------------------------------
//
// ---
constraint_item AppConnectSegmentsNode::Formulate(GCE_system solver)
{
  geom_item ends[2] = { GCE_PointOf(solver, crvItem1, GCE_SECOND_END),
                        GCE_PointOf(solver, crvItem2, GCE_FIRST_END) };  // end points
  return GCE_AddCoincidence(solver, ends);
}

//----------------------------------------------------------------------------------------
//
// ---
AppPerpendicularNode::AppPerpendicularNode(AppGeomNodeCRef gNode1, AppGeomNodeCRef gNode2)
  : AppConstraintNode()
  , lineItem1(gNode1.GceItem())
  , lineItem2(gNode2.GceItem())
{}

//----------------------------------------------------------------------------------------
//
// ---
constraint_item AppPerpendicularNode::Formulate(GCE_system solver)
{
  geom_item lines[2] = { lineItem1, lineItem2 };
  return GCE_AddPerpendicular(solver, lines);
}

//----------------------------------------------------------------------------------------
//
// ---
AppParallelNode::AppParallelNode(AppGeomNodeCRef gNode1, AppGeomNodeCRef gNode2)
    : AppConstraintNode()
    , lineItem1(gNode1.GceItem())
    , lineItem2(gNode2.GceItem())
{}

//----------------------------------------------------------------------------------------
//
// ---
constraint_item AppParallelNode::Formulate(GCE_system solver)
{
  geom_item lines[2] = { lineItem1, lineItem2 };
  return GCE_AddParallel(solver, lines);
}

//----------------------------------------------------------------------------------------
//
// ---
AppEqualRadiusNode::AppEqualRadiusNode(AppGeomNodeCRef gNode1, AppGeomNodeCRef gNode2)
  : AppConstraintNode()
  , geomItem1(gNode1.GceItem())
  , geomItem2(gNode2.GceItem())
{}

//----------------------------------------------------------------------------------------
//
// ---
constraint_item AppEqualRadiusNode::Formulate(GCE_system solver)
{
  return GCE_AddEqualRadius(solver, geomItem1, geomItem2);
}
