declare class Sample {
    // 返り値の型を指定し忘れている！
    // error TS7010: 'method', which lacks return-type annotation,
    //               implicitly has an 'any' return type.
    method();
}
