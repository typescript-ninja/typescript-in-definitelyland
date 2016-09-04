// UMD形式のライブラリがglobalに展開されたときの動作に相当する
// importした時、普通のモジュールとして振る舞う
import { randomizeString } from "strutil";
import { happy } from "strutil-extra";

randomizeString("TypeScript");
happy("TypeScript");

// strutil-extra で追加したパラメータも反映されている
randomizeString("TypeScript", {
	i: 11,
	reverse: true, // これ
});

// globalのStringも拡張されている
"TypeScript".randomizeString();
"TypeScript".happy();
