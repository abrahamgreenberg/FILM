define bu
    tcl
    shell make debug
end

define reb
    bu
    re
end

define ldt
    shell rm -r "[01] test"
    shell cp -r "[02] template" "[01] test"
end 