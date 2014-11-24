"use strict";

class Sample {
    test():void {
        var funcA = () => {
            // ここでの this は元のまま(Sampleのインスタンス)
            console.log(typeof this);
        };
        var funcB = function () {
            // ここでの this は undefined (ECMAScriptの仕様)
            console.log(typeof this);
        };
        // object と表示される
        funcA();
        // undefined と表示される
        funcB();
    }
}

new Sample().test();
