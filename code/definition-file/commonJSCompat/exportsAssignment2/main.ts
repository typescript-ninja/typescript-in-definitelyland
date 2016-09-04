// モジュール全体をutilに割当て
import * as util from "./util";

// この書き方は誤り util.ts にdefaultエクスポートはない
// error TS1192: Module '"略/util"' has no default export.
// import util from "./util";

// Hello, CommonJS と表示される
util("CommonJS");
