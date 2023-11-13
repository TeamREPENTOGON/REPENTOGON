const fs = require("fs");
const path = require("path");

const allFiles = getFilesForDirectory("./documentation/docs/");
const defaultVariables = "int|char|static|const|userdata|table|void|boolean|float|string|Object|function";
const ignoreVars = "LRoomAreaDesc|LRoomTileDesc|Ambush|ItemOverlay|HomingLaser|LaserHomingType|SkinColor|PlayerPocketItem|DoorSet|MultiShotParams|EntityDesc|Ending|FadeoutTarget";

const regularExpressions = [
  ["Required spaces in headings", /#{4} \w+ \w+ \(\w/],
  ["Required spaces in code blocks", /```\w+\n\s+\w+/],
  ["Function header has spaces between brackets", /^#{3} .*\(\s+\)/],
  ["Variable definitions that are falsely marked as functions", /#{4}(.)*([^\)]\s\{:)(.)*(\bFunctions\b)/],
  ["Function declaration with no arguments does not have spaces in between the brackets", /#{4}(.)*\(\)[^,]/],
  ["Variables with function or links in the title", /#{3}(.)*([\(\)]\s\{:)(.)*(\bVariables\b)/],
  ['Function header name is not split with middle dot characters "·"', /^#{3} .*([a-z][^·_][A-Z])+.*Functions/],
  ["Headers should not have a link in them", /^#{3}\s\[/],
  ["Link to return value is missing", new RegExp("#{4} (const\\s|static\\s)*(?!\\[)(?!" + defaultVariables + "|" + ignoreVars + ")")],
  ["Function declaration first argument has no link", new RegExp("#{4}(.)*\\w \\( (?!" + defaultVariables + "|" + ignoreVars + "|\\)|\\[)")],
  ["Function declaration later argument has no link", new RegExp("#{4}(.)*\\w \\(.*,(?!\\s[" + defaultVariables + "|" + ignoreVars + "|(\\[.*\\]\\(.*\\))])")],
  ["Badge has more type-definitions than allowed", /\[ \]\(#\)\{: ((.static|.const|.abrep|.rep|.abp)\s){2,5}.tooltip .badge \}/],
  ["Internal link doesnt work because its CamelCase.", /\[\w+\]\(([a-z][^·_][A-Z])*#[A-Z]\w+[A-Z]*/],
];


var hasError = false;

for (const file of allFiles) {
  const fileContents = getFileContents(file);
  const lines = fileContents.split("\n");
  let constructors = [];
  let functions = [];
  let variables = [];
  let modConstructors = [];
  let modFunctions = [];
  let modVariables = [];

  for (let index = 0; index < lines.length; index++) {
    const line = lines[index];
    if (line.trim().startsWith("### ")) {
      let cleanedLine = line.replaceAll("### ", "").replaceAll("·", "").split("{")[0].split("(")[0].trim();
      cleanedLine = cleanedLine.replaceAll("__", "zzzzz"); // Change sort priority to list underscore functions below normal ones
      if (line.includes("Constructors")) {
        if(line.includes("Modified"))
          modConstructors.push(cleanedLine);
        else
          constructors.push(cleanedLine);
      } else if (line.includes("Functions")) {
        if(line.includes("Modified"))
          modFunctions.push(cleanedLine);
        else
        functions.push(cleanedLine);
      } else if (line.includes("Variables")) {
        if(line.includes("Modified"))
          modVariables.push(cleanedLine);
        else
        variables.push(cleanedLine);
      }
    }

    for (const regex of regularExpressions) {
      if (regex[1].test(line) === false) {
        continue;
      }

      console.log(`${file} at line ${index + 1}: ${regex[0]}`);
      console.log(`\tLine: ` + regex[1].exec(line).input);
      hasError = true;
    }
  }

  // check if functions are sorted alphabetically
  checkSort(file, constructors);
  checkSort(file, functions);
  checkSort(file, variables);
  checkSort(file, modConstructors);
  checkSort(file, modFunctions);
  checkSort(file, modVariables);
}

function checkSort(file, entryList){
  let sortedList = entryList.slice();
  sortedList.sort(function (a, b) {
    return a.toLowerCase().localeCompare(b.toLowerCase());
  });
  for (let index = 0; index < sortedList.length; index++) {
    let entry = entryList[index];
    if (entry.toLowerCase() != sortedList[index].toLowerCase()) {
      var diff = Math.abs(index - sortedList.indexOf(entry));
      console.log(`${file}: Entry '` + entry + `' is not sorted correctly! Its at location ` + index+ ` but should be at ` + sortedList.indexOf(entry) + (diff >1?"\t !!!":""));
      //hasError = true;   // ignore if error. sort is not that important to prevent build
    }
  }
}

function getFilesForDirectory(directory) {
  const foundFiles = [];
  const files = fs.readdirSync(directory);

  files.forEach(function (file) {
    const filePath = path.join(directory, file);

    if (fs.statSync(filePath).isDirectory() === false && file.match(/\.md$/)) {
      foundFiles.push(filePath);
    }
  });

  return foundFiles.map((file) => path.resolve(process.cwd(), file));
}

function getFileContents(filepath) {
  return fs.readFileSync(filepath, "utf8");
}

if (hasError) {
  console.log("Some Code checks failed!");
  const core = require("@actions/core");
  core.setFailed("Some Code checks failed!");
} else {
  console.log("All code checks run successfully!");
}
