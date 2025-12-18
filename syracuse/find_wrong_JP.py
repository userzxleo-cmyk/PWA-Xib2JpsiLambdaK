import re
import glob
import argparse

def parse_args():
    parser = argparse.ArgumentParser(description="Find submissions with wrong JP")
    parser.add_argument("--glob", help="Glob pattern to find submission files", type=str, required=True)
    parser.add_argument("--outfile", help="Output file to dump tags with wrong JP hypothesis", type=str, required=True)
    args = parser.parse_args()
    return args

def get_tags( filepath ):
    tags = list()
    pattern = re.compile(r"\s+(default.*), ([1-3]) ([1-3]) default.*")
    Pc4440_pattern = re.compile(r"default_4440m([1-3])")
    Pc4457_pattern = re.compile(r".*-4457m([1-3])")
    with open(filepath, "r") as file:
        for line in file.readlines():
            match = pattern.match(line)
            if match:
                tag = match.group(1)
                Pc4440_setting = match.group(2)
                Pc4457_setting = match.group(3)
                Pc4440_tag = Pc4440_pattern.match(tag).group(1)
                Pc4457_tag = Pc4457_pattern.match(tag).group(1)

                if Pc4440_setting != Pc4440_tag and Pc4457_setting != Pc4457_tag:
                    tags.append(match.group(1))

    return tags

if __name__ == "__main__":
    args = parse_args()
    filepaths = glob.glob(args.glob)
    tags = [ get_tags( path ) for path in filepaths ]
    tags = [ tag for subtags in tags for tag in subtags ]
    with open(args.outfile, "w") as file:
        for tag in tags:
            file.write(f"{tag}\n")

