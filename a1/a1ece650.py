#!/usr/bin/env python3
import sys
import re

# Define a Point class representing a point in 2D space
class Point(object):
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)

        self.point = '(' + str(x) + ',' + str(y) + ')'
        self.point_list = []

    def __repr__(self):
        return '({0:.2f},{1:.2f})'.format(self.x, self.y)

# Define a Line class representing a line segment in the format 
class Line(object):
    def __init__(self, p1, p2):
        self.src = p1
        self.dst = p2

    def __repr__(self):
        return repr(self.src) + '-->' + repr(self.dst)

# Function to calculate the intersection point of two line segments
def intersect(l1, l2):
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y

    x_num = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4))
    x_den = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))

    y_num = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)
    y_den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)
    if x_den != 0 and y_den != 0:
        x_coor = x_num / x_den
        y_coor = y_num / y_den
    

        if (min(x1, x2) <= x_coor <= max(x1, x2) and min(y1, y2) <= y_coor <= max(y1, y2)):
            if (min(x3, x4) <= x_coor <= max(x3, x4) and min(y3, y4) <= y_coor <= max(y3, y4)):
                return Point(x_coor, y_coor)
    else:
        return None


# Function to generate vertices and edges from street data
def generate_vertices_and_edges(streets_final):
    streets_names = list(streets_final.keys()) 

    vertex_id = 0
    vertices_dict = {}
    list_vertices = []
    edges_db = []
    intersection_points = []
    intersection_db = {}

    

    for i in range(len(streets_names) - 1):
        for j in range(i + 1, len(streets_names)):
            point_1 = streets_final[streets_names[i]]
            point_2 = streets_final[streets_names[j]]

            for line1 in point_1:
                for line2 in point_2:
                    intersection_point = intersect(line1, line2)

                    if intersection_point is not None:
                        intersection_db.setdefault(intersection_point, []).append(line2)
                        intersection_db.setdefault(intersection_point, []).append(line1)

                        if not str(intersection_point) in intersection_points:
                            intersection_points.append(intersection_point)
                        else:
                            pass

                        for element in [line1.dst,line1.src,line2.dst,line2.src]:
                            if str(element) in list_vertices:
                                pass
                            
                            else:
                              vertex_id = vertex_id + 1
                              vertices_dict[vertex_id] = element
                              list_vertices.append(str(element))

                        # add the intersections
                        if str(intersection_point) in list_vertices:
                            pass
                        else:
                            vertex_id = vertex_id + 1
                            vertices_dict[vertex_id] = intersection_point
                            list_vertices.append(str(intersection_point))
                            
    print('V = {')
    for key in vertices_dict:
        print(str(key) + ": " + str(vertices_dict[key]))
    print('}')
    
    
    
    # Now we will generate the edges


    for i in range(len(intersection_points) - 1):
        for j in range(i + 1, len(intersection_points)):
          if intersection_points[j].x < intersection_points[i].x:
                intersection_points[j], intersection_points[i] = intersection_points[i], intersection_points[j]
          elif intersection_points[j].x == intersection_points[i].x and intersection_points[j].y < intersection_points[i].y:
                intersection_points[j], intersection_points[i] = intersection_points[i], intersection_points[j]

    for k in range(len(streets_names)):
        street = streets_final[streets_names[k]]
        for line in street:
          endpoint1, endpoint2 = line.src, line.dst
          if endpoint1.x > endpoint2.x:
                endpoint1, endpoint2 = endpoint2, endpoint1
          elif endpoint1.x == endpoint2.x and endpoint1.y > endpoint2.y:
                endpoint1, endpoint2 = endpoint2, endpoint1

          result_list = []

          for inter_values in intersection_points:
                i_value_key = intersection_db[inter_values]
                for value in i_value_key:
                    if value == line:
                        if str(endpoint1) not in result_list:
                            result_list.append(str(endpoint1))
                        if str(inter_values) not in result_list:
                            result_list.append(str(inter_values))

          if str(endpoint2) not in result_list:
                result_list.append(str(endpoint2))


          for m in range(len(result_list) - 1):
                edge = Line(result_list[m], result_list[m + 1])
                if str(edge) not in edges_db:
                    edges_db.append(edge)
                    
    updated_vertices = {str(v): k for k, v in vertices_dict.items()}

    print('E = {')
    count = 0
    for edge in edges_db:
        id_a = updated_vertices[edge.src]
        id_b = updated_vertices[edge.dst]
        count += 1
        if count < len(edges_db):
            print("<" + str(id_a) + "," + str(id_b) + ">")
        else:
            print("<" + str(id_a) + "," + str(id_b) + ">")

    print('}')



# Function to format street coordinates into a required format
def format_coordinates(streetdb):
    streetdbfinal = {}

    for streets_names, coordinates in streetdb.items():
        segments = []
        for i in range(len(coordinates) - 1):
            coord1 = coordinates[i]
            coord2 = coordinates[i + 1]
            p1 = Point(coord1[0], coord1[1])
            p2 = Point(coord2[0], coord2[1])
            line = Line(p1, p2)
            segments.append((line))

        streetdbfinal[streets_names] = segments


    return streetdbfinal

# Define streetdb dictionary
streetdb = {}


# Function to add a street in streetdb
def add_street(street_name, coordinates):
    if len(coordinates) < 2:
        raise Exception("Error! There should be a minimum of 2 coordinates")
    else:
        street_name = street_name.replace('"', '')
        streetdb[street_name] = coordinates
        
# Function to modify a street in streetdb
def modify_street(street_name, coordinates):
    if len(coordinates) < 2:
        raise Exception("Error! There should be a minimum of 2 coordinates")
    else:
        street_name = street_name.replace('"', '')
        streetdb[street_name] = coordinates

# Function to remove a street from streetdb
def remove_street(street_name):
    if street_name in streetdb:
      del streetdb[street_name]
    else:
      raise Exception("Error: Street name " + street_name + " does not exists!")


# Function to generate and plot a graph based on street data
def graph_generator(streetdb):
    streets_formatted = format_coordinates(streetdb)
    if streets_formatted != {}:
        pass
    else:
        raise Exception('Error: the set is empty')
    generate_vertices_and_edges(streets_formatted)


def main():
    while True:
        line = sys.stdin.readline()
        if line.strip() == "":
            break
        try:
            if line.strip() == "0":
                break
            line = line.strip() 
            command = r'\b(add|rm|mod|gg)\s*((?: +"[a-zA-Z0-9\s.,\'#&\/*+\-]+?" *?))?((?:\(-?\d+(\.\d+)?,-?\d+(\.\d+)?\) *)+)?$'
            command_split_var = re.match(command, line)
            if command_split_var:
                to_do = command_split_var.group(1)
                street_name = command_split_var.group(2)
                coor = command_split_var.group(3)

            else:
                raise Exception("Error: Command is not in the format,  Please enter in correct format")
                continue

            if to_do:
                  to_do.strip()

            if coor:
                coordinate_pairs = re.findall(r'\(([^)]+)\)', coor)

                # Process each coordinate pair
                coordinates_float = [tuple(map(float, pair.split(','))) for pair in coordinate_pairs]

            
            if street_name: 
                street_name = street_name.strip()
                street_name = street_name.replace('"','')
            try:
                if to_do == 'add':
                    if not street_name:
                        raise Exception("Error: you have not entered the street name")
                    elif street_name in streetdb:
                        raise Exception("Error:" + street_name + "in the Database already exists")
                    elif coor:
                        add_street(street_name, coordinates_float)
                    else:
                        raise Exception("Error: The input does not follow the correct format")
                elif to_do == 'mod':
                    if not street_name:
                        raise Exception("Error: you have not entered the street name")
                    elif street_name not in streetdb:
                        raise Exception("Error: Street" + street_name +  "does not exist")
                    elif coor:
                        modify_street(street_name, coordinates_float)
                    else:
                        raise Exception("Error: The input does not follow the correct format")
                elif to_do == 'rm':
                    # if len(coor) != 0:
                    #     raise Exception("Error: Command is not in proper Format")
                    if not street_name:
                        raise Exception("Error: you have not entered the street name")
                    else:
                        remove_street(street_name)
                elif to_do == 'gg':
                    graph_generator(streetdb)
                else:
                    raise Exception("Error: Wrong Command Entered ")
            except Exception as error_:
               print( str(error_), file= sys.stderr)
        
        
        except Exception as error_:
            print('Error: ' + str(error_), file= sys.stderr)
                   
    sys.exit(0)
    
if __name__ == "__main__":
    main()
