-- DO NOT ALTER THIS FILE. IF YOU DO, AUSTRALIA WILL BE SWALLOWED UP BY A
-- VOLCANO, AND YOU WILL HAVE BAD KARMA FOR LIFE. OH, AND THE GAME WILL BREAK.



-- Lua Sandbox

-- This is strictly to create a safe Lua environment to prevent malicious
-- scripts from doing any damage to the system.
arg = nil
debug.debug = nil
debug.getfenv = getfenv
debug.getregistry = nil
dofile = nil
io = { write = wcLog }
loadfile = nil
os.execute = nil
os.getenv = nil
os.remove = nil
os.rename = nil
os.tmpname = nil
package.loaded.io = io
package.loaded.package = nil
package = nil
require = nil

