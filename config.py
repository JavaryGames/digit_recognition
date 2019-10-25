
def can_build(env, platform):
    return True

def binary_data_header_builder(target, source, env):
    import os
    for f in source:
        source_file = open(str(f), "rb")
        target_file = open(str(f) + ".gen.h", "w")

        target_file.write("/* WARNING, THIS FILE WAS GENERATED, DO NOT EDIT */\n")
        header_guard = (os.path.basename(str(f)).replace(".", "_") + "_H").upper()
        target_file.write("#ifndef " + header_guard + "\n")
        target_file.write("#define " + header_guard + "\n")

        target_file.write("""

const unsigned char MLP_KNOWLEDGE[] = {""")

        gen = ""
        byte = source_file.read(1)
        while len(byte) == 1:
            gen += hex(ord(byte))
            byte = source_file.read(1)
            if (len(byte) == 1):
                gen += ","

        target_file.write(gen)
        target_file.write("""};
""")
        target_file.write("#endif // " + header_guard + "\n")

        target_file.close()
        source_file.close()

def configure(env):
    env.Append(BUILDERS =
        { "MLP_KNOWLEDGE" : env.Builder(action=binary_data_header_builder, suffix=".knw.gen.h", src_suffix=".knw") }
    )
