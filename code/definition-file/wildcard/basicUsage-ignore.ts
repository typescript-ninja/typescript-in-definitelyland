import * as _ from "lodash";
import * as sample from "sample/foobar";
import * as data from "json!./bar.json";
import * as text from "./foo.txt!text";

// _はany
_.map([1, 2, 3], n => n * 3);
// $はany
$("#id");
// sampleはany
sample;
// dataもany
data;
// textはstring
text.toUpperCase();
