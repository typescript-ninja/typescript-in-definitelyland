var obj: number | string | boolean;

if (typeof obj === "string") {
    // ここでは string と確定されている！
    obj.charAt(0);
} else {
    // ここでは string が引かれ number | boolean;
    obj;
}

if (typeof obj === "string") {
    // ここでは string と確定されている！
    obj.charAt(0);
} else if(typeof obj === "number") {
    // ここでは number と確定されている！
    obj.toFixed(2);
} else {
    // ここでは string, number が引かれ boolean となる！
    obj;
}
