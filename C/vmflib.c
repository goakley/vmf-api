#include <stdio.h>
#include <stdlib.h>
#include "vmflib.h"

// ---
// BEGIN 'CONSTRUCTOR' DEFINITIONS

vmf_vmf buildVMF(vmf_versioninfo versioninfo, 
		 vmf_world world)
{
  vmf_vmf result = {versioninfo, world};
  return result;
}

/**
 * Constructs a vmf_versioninfo, which holds the version information 
 * for a .vmf file.
 * Any argument with a value of -1 will be given a default value
 */
vmf_versioninfo buildVMF_versioninfo(int editorversion, 
				     int editorbuild, 
				     int mapversion, 
				     int formatversion, 
				     int prefab)
{
  vmf_versioninfo result = 
    {
      // defaults to 400
      (editorversion==-1?400:editorversion), 
      // defaults to 5439
      (editorbuild==-1?5439:editorbuild),
      // defaults to 1
      (mapversion==-1?1:mapversion),
      // defaults to 100
      (formatversion==-1?100:formatversion),
      // defaults to 0
      (prefab==-1?0:prefab)
    };
  return result;
}

/**
 * Constructs a vmf_world, which holds the information for a world 
 * section in a .vmf file.
 * Any char* argument that is given a null pointer will take on a default 
 * value.
 */
vmf_world buildVMF_world(int mapversion,
			 char* classname,
			 char* skyname,
			 int maxpropscreenwidth,
			 char* detailvbsp,
			 char* detailmaterial)
{
  vmf_world result = 
    {
      vmf_id_counter++,
      (mapversion==-1?1:mapversion),
      (classname==NULL?"worldspawn":classname),
      (skyname==NULL?"sky_day01_01":skyname),
      maxpropscreenwidth, // -1 is the default maxpropscreenwidth anyway!
      (detailvbsp==NULL?"detail.vbsp":detailvbsp),
      (detailmaterial==NULL?"detail/detailsprites":detailmaterial),
      malloc(2048*sizeof(vmf_solid)),
      0,
      2048
    };
  return result;
}

vmf_solid buildVMF_solid(vmf_editor editor)
{
  vmf_solid result = 
    {
      .id = vmf_id_counter++,
      // .sides is already initialized (it's a static array)
      .editor = editor
    };
  return result;
}

vmf_side buildVMF_side(vmf_point_3d_d plane_1, 
		       vmf_point_3d_d plane_2, 
		       vmf_point_3d_d plane_3, 
		       char* material, 
		       vmf_point_4d_d uaxis_c, double uaxis_s, 
		       vmf_point_4d_d vaxis_c, double vaxis_s,
		       double rotation, 
		       int lightmapscale,
		       int smoothing_groups)
{
  vmf_side result =
    {
      .id = vmf_id_counter++,
      // .plane is already initialized (it's a static 2-d array)
      .material = (material==NULL?"PLASTIC/PLASTICWALL001A":material),
      .uaxis_c = uaxis_c,
      .uaxis_s = uaxis_s, // !!!NOT DEFAULTED!!!
      .vaxis_c = vaxis_c,
      .vaxis_s = vaxis_s, // !!!NOT DEFAULTED!!!
      .rotation = rotation, // !!!NOT DEFAULTED!!!
      .lightmapscale = (lightmapscale==-1?16:lightmapscale),
      .smoothing_groups = smoothing_groups // !!!NOT DEFAULTED!!!
    };
  result.planes[0] = plane_1;
  result.planes[1] = plane_2;
  result.planes[2] = plane_3;
  return result;
}

vmf_editor buildVMF_editor(vmf_point_3i_i color,
			   int visgroupshown,
			   int visgroupautoshown)
{
  vmf_editor result = 
    {
      color,
      (visgroupshown==0?0:1), // if non-zero, make it 1 (!NOT DEFAULTED!)
      (visgroupautoshown==0?0:1) // if non-zero, make it 1 (!NOT DEFAULTED!)
    };
  return result;
}

// BEGIN 'CONTRUCTOR' DEFINITIONS
// ---

// ---
// BEGIN FUNCTION DEFINITIONS

void vmf_shape_setSide(vmf_solid *solid, vmf_side side, int id)
{
  if (id < 1 || id > 6)
    return;
  solid->sides[id-1] = side;
}

void vmf_world_addSolid(vmf_world *world, vmf_solid solid)
{
  if ((world->_solid_size)>=(world->_solid_capacity))
    {
      // make the dynamic array bigger
      int i;
      vmf_solid *newSolids = malloc(1.5*(world->_solid_capacity));
      vmf_solid *oldSolids = world->solids;
      for (i = 0; i < (world->_solid_capacity); i++)
	newSolids[i] = oldSolids[i];
      free(oldSolids); oldSolids = NULL;
      world->solids = newSolids; newSolids = NULL;
      world->_solid_capacity *= 1.5;
    }
  (world->solids)[world->_solid_size++] = solid;
}

// END FUNCTION DEFINITIONS
// ---

// ---
// BEGIN PRINTER FUNCTION

void vmf_fprint(FILE *f, vmf_vmf vmf)
{
  // ---
  // PRINT VERSION INFO
  fprintf(f, "versioninfo\n{\n");
  fprintf(f, "\t\"editorversion\" \"%d\"\n", vmf.versioninfo.editorversion);
  fprintf(f, "\t\"editorbuild\" \"%d\"\n", vmf.versioninfo.editorbuild);
  fprintf(f, "\t\"mapversion\" \"%d\"\n", vmf.versioninfo.mapversion);
  fprintf(f, "\t\"formatversion\" \"%d\"\n", vmf.versioninfo.formatversion);
  fprintf(f, "\t\"prefab\" \"%d\"\n}\n", vmf.versioninfo.prefab);
  // END PRINT VERSION INFO ---
  // ---
  // PRINT WORLD INFO
  fprintf(f, "world\n{\n");
  fprintf(f, "\t\"id\" \"%d\"\n", vmf.world.id);
  fprintf(f, "\t\"mapversion\" \"%d\"\n", vmf.world.mapversion);
  fprintf(f, "\t\"classname\" \"%s\"\n", vmf.world.classname);
  fprintf(f, "\t\"detailmaterial\" \"%s\"\n", vmf.world.detailmaterial);
  fprintf(f, "\t\"detailvbsp\" \"%s\"\n", vmf.world.detailvbsp);
  fprintf(f, "\t\"maxpropscreenwidth\" \"%d\"\n",vmf.world.maxpropscreenwidth);
  fprintf(f, "\t\"skyname\" \"%s\"\n", vmf.world.skyname);
  // ---
  // PRINT SOLIDS
  int i, j;
  for (i = 0; i < ((vmf.world)._solid_size); i++)
    {
      vmf_solid solid = (vmf.world).solids[i];
      fprintf(f, "\tsolid\n\t{\n");
      fprintf(f, "\t\t\"id\" \"%d\"\n", solid.id);
      // print through the sides
      for (j = 0; j < 6; j++)
	{
	  vmf_side side = solid.sides[j];
	  fprintf(f, "\t\tside\n\t\t{\n");
	  fprintf(f, "\t\t\t\"id\" \"%d\"\n", side.id);
	  fprintf(f, "\t\t\t\"plane\" \"(%f %f %f) (%f %f %f) (%f %f %f)\"\n",
		  side.planes[0].x,side.planes[0].y,side.planes[0].z,
		  side.planes[1].x,side.planes[1].y,side.planes[1].z,
		  side.planes[2].x,side.planes[2].y,side.planes[2].z);
	  fprintf(f, "\t\t\t\"material\" \"%s\"\n", side.material);
	  fprintf(f, "\t\t\t\"uaxis\" \"[%f %f %f %f] %f\"\n", 
		  side.uaxis_c.x, side.uaxis_c.y, side.uaxis_c.z, 
		  side.uaxis_c.w, side.uaxis_s);
	  fprintf(f, "\t\t\t\"vaxis\" \"[%f %f %f %f] %f\"\n", 
		  side.vaxis_c.x, side.vaxis_c.y, side.vaxis_c.z, 
		  side.vaxis_c.w, side.vaxis_s);
	  fprintf(f, "\t\t\t\"rotation\" \"%f\"\n", side.rotation);
	  fprintf(f, "\t\t\t\"lightmapscale\" \"%d\"\n", side.lightmapscale);
	  fprintf(f, "\t\t\t\"smoothing_groups\" \"%d\"\n", 
		  side.smoothing_groups);
	  fprintf(f, "\t\t}\n");
	}
      fprintf(f, "\t}\n");
    }
  // END PRINT SOLIDS ---
  fprintf(f, "}\n");
  // END PRINT WORLD INFO ---
}

// END PRINTER FUNCTION
// ---
