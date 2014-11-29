/// <reference path="./reference-a.ts" />

module b {
    console.log(a.hello("internal module"));
}
