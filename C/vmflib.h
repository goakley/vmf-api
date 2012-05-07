#ifndef VMFLIB_H
#define VMFLIB_H

#include <stdio.h>

unsigned int vmf_id_counter = 1;

// ---
// BEGIN STRUCT DECLARATIONS

/**
 * Defines a point in 3D space, whose coordinates are represented by 
 * integer values.
 */
typedef struct vmf_point_3i_i
{
  int x;
  int y;
  int z;
} vmf_point_3i_i;

/**
 * Defines a point in 3D space, whose coordinates are represented by 
 * double-precision floating point values.
 */
typedef struct vmf_point_3d_d
{
  double x;
  double y;
  double z;
} vmf_point_3d_d;

/**
 * Defines a point in 4D space, whose coordinates are represented by 
 * double-precision floating point values.
 */
typedef struct vmf_point_4d_d
{
  double x;
  double y;
  double z;
  double w;
} vmf_point_4d_d;

/**
 * Represents the versioninfo section of a .vmf file
 * , which can be found at the beginning of the file.  It is used to 
 * describe the Hammer editor environment that is compatible with 
 * the .vmf file.
 */
typedef struct vmf_versioninfo
{
  /** Describes the version of Hammer that this file is compatible with */
  int editorversion;
  /** Describes the build of Hammer that this file is compatible with */
  int editorbuild;
  /** Describes the number of times this map has been saved */
  int mapversion;
  /**/
  int formatversion;
  /**/
  int prefab;
} vmf_versioninfo;
/** A vmf_versioninfo object with default values; NOT for direct use */
const vmf_versioninfo vmf_versioninfo_default_struct = 
  {400, 439, 1, 100, 0};
/**
 * Returns a functional default vmf_versioninfo object.
 */
vmf_versioninfo vmf_versioninfo_default()
{
  return vmf_versioninfo_default_struct;
}

/**
 * Represents the side section of a .vmf file.
 * This is used to hold information about the side of a solid (another 
 * vmf object).
 */
typedef struct vmf_side
{
  /** The ID number of the side.  Note that no two sides can have the same
      ID number. */
  int id;
  /** Holds the three points that determine the orientation of this side, 
      which is a plane. */
  vmf_point_3d_d plane_points[3];
  /** Describes the material that is mapped to this side as defined by Hammer, 
      e.g. "PLASTIC/PALSTICWALL001A" for plastic wall 1A. */
  char* material;
  /**/
  vmf_point_4d_d uaxis_c;
  /**/
  double uaxis_s;
  /**/
  vmf_point_4d_d vaxis_c;
  /**/
  double vaxis_s;
  /**/
  double rotation;
  /**/
  int lightmapscale;
  /**/
  int smoothing_groups;
} vmf_side;
/** A vmf_side object with default values; NOT for direct use */
const vmf_side vmf_side_default_struct = 
  {.material="BRICK/BRICKFLOOR001A", 
   .plane_points={{0,0,0},{0,0,0},{0,0,0}}, 
   .uaxis_c={1,0,0,0}, .uaxis_s=0.25, 
   .vaxis_c={0,0,-1,0}, .vaxis_s=0.25, .rotation=0, .lightmapscale=16, 
   .smoothing_groups = 0};
/**
 * Returns a functional default vmf_side object.
 */
vmf_side vmf_side_default()
{
  // set up the ID
  vmf_side result = vmf_side_default_struct;
  result.id = vmf_id_counter++;
  return result;
}

/**
 * Represents a editor section in a .vmf file
 * , used in multiple places throughout the file.
 */
typedef struct vmf_editor
{
  /** The colour associated with this editor, represented by 3 points, 
      Red, Green, and Blue.*/
  vmf_point_3i_i color;
  /**/
  int visgroupshown;
  /**/
  int visgroupautoshown;
} vmf_editor;
/** A vmf_editor object with default values; NOT for direct use */
const vmf_editor vmf_editor_default_struct = 
  {{127,127,127}, 1, 1};
/**
 * Returns a functional default vmf_side object.
 */
vmf_editor vmf_editor_default()
{
  return vmf_editor_default_struct;
}

/**
 * Represents a solid section in a .vmf file
 * .  A solid is a physical object in a vmf, made up of sides and 
 * associated with an editor.
 */
typedef struct vmf_solid
{
  /** The ID number of this solid.  Should be unique from other IDs. */
  int id;
  /** Stores the 6 different side objects that make up the geometry 
      for this solid */
  struct vmf_side sides[6];
  /** Describes the editor associated with this solid */
  struct vmf_editor editor;
} vmf_solid;
/** A vmf_solid object with default values; NOT for direct use */
const vmf_solid vmf_solid_default_struct = {};
/**
 * Returns a functional default vmf_solid object.
 */
vmf_solid vmf_solid_default()
{
  vmf_solid result = vmf_solid_default_struct;
  // set up the ID
  result.id = vmf_id_counter++;
  // set up the sides
  int i;
  for(i = 0; i < 6; i++)
    result.sides[i] = vmf_side_default();
  // set up the editor
  result.editor = vmf_editor_default();
  return result;
}

struct vmf_node_solid
{
  vmf_solid solid;
  struct vmf_node_solid *next;
};
typedef struct vmf_list_solid
{
  struct vmf_node_solid *head;
  unsigned long long int size;
} vmf_list_solid;

/**
 * Represents a world in a vmf file
 * , which is made up of things such as solids.
 */
typedef struct vmf_world
{
  /** The ID number of this world, which should be unique to other IDs */
  int id;
  /**/
  int mapversion;
  /**/
  char* classname;
  /** Describes the skybox used with this world as defined by hammer, 
      e.g. "sky_day_01_01" for the first daytime skybox. */
  char* skyname;
  /**/
  int maxpropscreenwidth;
  /**/
  char* detailvbsp;
  /**/
  char* detailmaterial;
  /** An list of all the solid objects that are contained by this world */
  vmf_list_solid solids;
} vmf_world;
/** A vmf_world object with default values; NOT for direct use */
const vmf_world vmf_world_default_struct = 
  {.mapversion=1, .classname="worldspawn", .skyname="sky_day01_01", 
   .maxpropscreenwidth=-1, .detailvbsp="detail.vbsp", 
   .detailmaterial="detail/detailsprites", .solids={NULL, 0}};
/**
 * Returns a functional default vmf_world object.
 */
vmf_world vmf_world_default()
{
  // set up the ID
  vmf_world result = vmf_world_default_struct;
  result.id = vmf_id_counter++;
  return result;
}

/**
 * Represents the overall .vmf file, containing everything that makes up 
 * a .vmf file.
 */
typedef struct vmf_vmf
{
  /** Describes the versioninfo associated with this vmf */
  struct vmf_versioninfo versioninfo;
  /** Describes the world associtaed with this vmf */
  struct vmf_world world;
} vmf_vmf;
/** A vmf_world object with default values; NOT for direct use */
const vmf_vmf vmf_vmf_default_struct = {};
/**
 * Returns a functional default vmf object.
 */
vmf_vmf vmf_vmf_default()
{
  vmf_vmf result = vmf_vmf_default_struct;
  // set up the versioninfo
  result.versioninfo = vmf_versioninfo_default();
  // set up the world
  result.world = vmf_world_default();
  return result;
}

// END STRUCT DECLARATIONS
// ---

// ---
// BEGIN 'CONSTRUCTOR' DECLARATIONS

/**
 * Constructs a vmf.
 * @param versioninfo The versioninfo for this object
 * @param world The world object associated with this vmf
 */
vmf_vmf vmf_build_vmf(vmf_versioninfo versioninfo, 
		      vmf_world world);

/**
 * Constructs a vmf_versioninfo, which holds the version information 
 * for a .vmf file.
 * Any argument with a value of -1 will be given a default value
 */
vmf_versioninfo vmf_build_versioninfo(int editorversion, 
				      int editorbuild, 
				      int mapversion, 
				      int formatversion, 
				      int prefab);

/**
 * Constructs a vmf_world.
 * Any char* argument that is given a null pointer will take on a default 
 * value.
 */
vmf_world vmf_build_world(int mapversion,
			  char* classname,
			  char* skyname,
			  int maxpropscreenwidth,
			  char* detailvbsp,
			  char* detailmaterial);

/**
 * Constructs a vmf_solid.
 * @param editor An editor to associate with this solid
 */
vmf_solid vmf_build_solid(vmf_editor editor);

/**
 * Constructs a vmf_side.
 */
vmf_side vmf_build_side(vmf_point_3d_d plane_1, 
			vmf_point_3d_d plane_2, 
			vmf_point_3d_d plane_3, 
			char* material, 
			vmf_point_4d_d uaxis_c, double uaxis_s, 
			vmf_point_4d_d vaxis_c, double vaxis_s,
			double rotation, 
			int lightmapscale,
			int smoothing_groups);

vmf_editor vmf_build_editor(vmf_point_3i_i color,
			    int visgroupshown,
			    int visgroupautoshown);

// END 'CONSTRUCTOR' DECLARATIONS
// ---

// ---
// BEGIN FUNCTION DEFINITIONS

/**
 * Sets the side of a solid to the value of a specific side.
 * The solid argument must not be a null pointer.
 * The ID argument must be a number between 1 and 6 inclusive, 
 * or the function will do nothing.
 */
void vmf_solid_setSide(vmf_solid *solid, vmf_side side, short id);

/**
 * Adds a solid to the specified world.
 * The world argument must not be a null pointer.
 */
void vmf_world_addSolid(vmf_world *world, vmf_solid solid);

/**
 * Outputs a vmf to a FILE.
 * The file argument must not be a null pointer.
 */
void vmf_fprint(FILE *f, vmf_vmf vmf);

// END FUNCTION DEFINITIONS
// ---

#endif
