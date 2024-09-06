import subprocess
import re

def get_include_paths():
    # 获取包含路径
    cmd = ['arm-none-eabi-gcc', '-v', '-E', '-x', 'c++', '-']
    process = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    _, stderr = process.communicate(input=b'')
    
    # 解析包含路径
    include_paths = []
    in_include_section = False
    for line in stderr.decode().splitlines():
        if line.startswith("#include <...> search starts here:"):
            in_include_section = True
        elif line.startswith("End of search list."):
            in_include_section = False
        elif in_include_section:
            include_paths.append(line.strip())
    
    return include_paths

def generate_clangd_file(include_paths):
    with open('.clangd', 'w') as f:
        f.write("CompileFlags:\n")
        f.write("  Add: [\n")
        f.write("    \"--target=arm-none-eabi\",\n")
        for path in include_paths:
            f.write(f"    \"-isystem{path}\",\n")
        f.write("  ]\n")
        f.write("  CompilationDatabase: \"build\"\n")
        f.write("  Compiler: arm-none-eabi\n")

def main():
    include_paths = get_include_paths()
    generate_clangd_file(include_paths)

if __name__ == "__main__":
    main()
