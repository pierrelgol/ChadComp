const std = @import("std");

pub fn main() void {
    std.debug.print("Hello, {s}", .{"World\n"});
    var v1: u16 = 1;
    var v2: u128 = 3;
    var v3: u128 = 5;

    std.debug.print("this is a variable u3 {d}, this is a variable u2 {d}, this is a variable u3 {d}", .{ v1, v2, v3 });
}
