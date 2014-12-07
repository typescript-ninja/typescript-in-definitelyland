// Genericsを使ってtupleを生成して返す
function zip<T1, T2>(v1: T1, v2: T2): [T1, T2] {
    return [v1, v2];
}

var tuple = zip("str", { hello: () => "Hello!" });
tuple[0].charAt(0); // おー、静的に検証される！
tuple[1].hello(); // おー、静的に検証される！
