import * as $ from "jquery";

// $ はany
$.notExists();

// これらもコンパイルが通る
import * as json from "json!package.json";
import * as sampleFoo from "sample/foo";
import * as sampleFooBar from "sample/foo/bar";

export { $, json, sampleFoo, sampleFooBar }
