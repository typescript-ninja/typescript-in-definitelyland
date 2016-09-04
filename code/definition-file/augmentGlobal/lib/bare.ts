// UMD形式のライブラリがglobalに展開されたときの動作に相当する
// import, export句がない場合、globalのstrutilが参照できる
strutil.randomizeString("TypeScript");
strutilExtra.happy("TypeScript");

// globalのStringも拡張されている
"TypeScript".randomizeString();
"TypeScript".happy();

// import、export が存在すると、ちゃんと読み込め！と怒られる
// error TS2686: Identifier 'strutil' must be imported from a module
// error TS2686: Identifier 'strutilExtra' must be imported from a module
