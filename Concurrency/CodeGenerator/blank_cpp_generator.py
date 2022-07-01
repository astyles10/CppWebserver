import sys
import yaml
import ast

# Either take command line arguments or read inputs from user
# Encapsulate features like threads, memory, I/o etc.
# Ask whether to use command line arguments

# Generate basic classes within a namespace
# Could write different constructor depending on pattern (singleton etc.)

# Could write a template in YAML, JSON etc. with class names, their functions & return types
# Allow setting default constructors, operators etc
# Allow deleting functions

# Ultimately would be interesting to convert natural language into simple functions
# E.g. "I would like to read file contents into a terminal"

class function_generator:
  def __init__(self, function_name: str, return_type: str, namespace: str = ""):
    self.name = function_name
    self.return_type = return_type
    if len(namespace):
      self.namespace = namespace

  def debug_print(self):
    function_print = str("{return_type} {name} () {{\nreturn 1;\n}}\n".format(return_type = self.return_type, name = self.name))
    print(function_print)

  # def print_class_function():

class class_generator:
  def __init__(self, class_name):
    self.name = class_name

  # def generate_function(self):
  #   class_file = open(f"{self.name}", "x") # Probably don't want to overwrite an existing class
  #   class_file.write("int main () \{ return 0;\}")

# def generate_function():

if __name__ == "__main__":
  if len(sys.argv) > 1:
    with open(sys.argv[1], 'r') as file:
      config_file = dict(yaml.safe_load(file))
    functions = dict(config_file['functions'])
    for function_name in functions.keys():
      function_properties = functions[function_name]
      generator = function_generator(function_name, function_properties['return_type'])
    
    # generator = function_generator("main", "int", "Namespace")
    generator.debug_print()
  else:
    print(f"Usage: {sys.argv[0]} <config_file.yaml>")
