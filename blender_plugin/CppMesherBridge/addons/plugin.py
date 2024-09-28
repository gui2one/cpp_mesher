bl_info = {
    "name": "Simple Cube Add-on",
    "blender": (3, 0, 0),
    "category": "Object",
    "version": (1, 0, 0),
    "author": "Your Name",
    "description": "A simple add-on that adds a cube",
}

import bpy
import os, sys
from queue import Queue
import threading
import time
import tempfile
from pathlib import Path
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

root_dir = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
print(root_dir)

# sys.exit(0)

from tcp.connect import send_command, wait_for_update, COMMAND

print(tempfile.gettempdir())
TEMP_MESH_PATH = os.path.join(tempfile.gettempdir(), "temp_mesh.ply")


def find_existing_target(context : bpy.types.Context) -> bpy.types.Object|None:
    all_objs : set[bpy.types.Object] = set(context.scene.objects)

    for obj in list(all_objs) :
        cur_name = obj.name
        if cur_name == "cpp_mesher_target" :
            return obj
        
    return None
# Define a new Panel
class VIEW3D_PT_CppMesherPanel(bpy.types.Panel):
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = "gui2one"
    bl_label = "Cpp Mesher Utils"

    def draw(self, context):
        row = self.layout.row()
        row.operator('object.create_cpp_mesher_target')

# Define a new operator
class CreateCppMesherTarget(bpy.types.Operator):
    bl_idname = "object.create_cpp_mesher_target"
    bl_label = "CppMesher Target"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):

        print("EXECUTE")
        existing = find_existing_target(context)
        if existing != None :
            print(existing)
            existing.select_set(state=True)
            
            try:
                bpy.data.objects.remove(existing)
            except Exception as e:
                print("Exception !!! ", e)
                pass


        all_objs = set(context.scene.objects)

        # send_command(COMMAND.MESH_FILE_UPDATE_BROADCAST)
        try : 
            # obj_path = wait_for_update().decode()
            bpy.ops.wm.ply_import(filepath=TEMP_MESH_PATH, merge_verts=False)
            
            imported : set[bpy.types.Object] = set(context.scene.objects) - all_objs
            print("Imported PLY file")
            for obj in list(imported) :
                obj.name = "cpp_mesher_target"

            obj = list(imported)[0]

            #create material
            mat_names = [mat.name for mat in bpy.data.materials]

            print(mat_names)
            if not obj.name in mat_names:
                material = bpy.data.materials.new(obj.name)
                obj.data.materials.append(material)
            else :
                for mat in bpy.data.materials :
                    if mat.name == obj.name :
                        obj.data.materials.append(mat)
                        break
                print("material already exists")                
        except Exception as e:
            print("oups", e)

        return {'FINISHED'}



classes = [
    CreateCppMesherTarget,
    VIEW3D_PT_CppMesherPanel
]

# Register the operator
def menu_func(self, context):
    self.layout.operator(CreateCppMesherTarget.bl_idname)

# Registration functions
def register():
    for cl in classes :
        bpy.utils.register_class(cl)
    bpy.types.VIEW3D_MT_mesh_add.append(menu_func)

def unregister():
    for cl in classes :
        bpy.utils.unregister_class(cl)
    bpy.types.VIEW3D_MT_mesh_add.remove(menu_func)


last_modification_time = 0
def check_file_and_run_operator():
    global last_modification_time
    
    # Get the current modification time of the file
    current_modification_time = os.path.getmtime(TEMP_MESH_PATH)
    
    # Compare it to the last known modification time
    if current_modification_time != last_modification_time:
        # If different, the file has been modified
        last_modification_time = current_modification_time
        
        # Run the operator or execute your custom code
        print("  ---->File has changed, running operator")
        bpy.ops.object.create_cpp_mesher_target()

    # Return the interval in seconds for the next check
    return 0.1  # This will run the check every 0.1s

# Register the timer function
bpy.app.timers.register(check_file_and_run_operator)

# Entry point
if __name__ == "__main__":
    register()


