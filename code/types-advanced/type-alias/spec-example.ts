// これらはinterfaceで表現不可 type aliasで正解
type StringOrNumber = string | number;
type TextObject = string | { text: string };
type Coord = [number, number];
type ObjectStatics = typeof Object;

// これらはinterfaceで表現可能
type HolidayLookup = Map<string, Date>;
interface AltHolidayLookup extends Map<string, Date> {
}

type Callback = (data: string) => void;
interface AltCallback {
	(date: string): void;
}

type RecFunc = () => RecFunc;
interface AltRecFunc {
	(): AltRecFunc;
}
