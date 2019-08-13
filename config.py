import os

def can_build(env, platform):
    return True

def data_header_builder(target, source, env):
    for f in source:
        source_file = open(str(f), "r")
        target_file = open(str(f) + ".gen.h", "w")

        target_file.write("/* WARNING, THIS FILE WAS GENERATED, DO NOT EDIT */\n")
        header_guard = (os.path.basename(str(f)).replace(".", "_") + "_H").upper()
        target_file.write("#ifndef " + header_guard + "\n")
        target_file.write("#define " + header_guard + "\n")

        target_file.write("""

#include <string>

std::string KNOWLEDGE = R"KNOWLEDGE(""")

        target_file.write(source_file.read())

        target_file.write(""")KNOWLEDGE";
""")

        target_file.write("#endif // " + header_guard + "\n")

        target_file.close()
        source_file.close()

def configure(env):
    env.Append(BUILDERS =
        { "NN_KNOWLEDGE" : env.Builder(action=data_header_builder, suffix=".knw.gen.h", src_suffix=".knw") }
    )
