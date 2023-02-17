echo "echo Restoring environment" > "/home/rove/Projects/tfs/cmake-build-release/deactivate_conanrunenv-release-x86_64.sh"
for v in 
do
    is_defined="true"
    value=$(printenv $v) || is_defined="" || true
    if [ -n "$value" ] || [ -n "$is_defined" ]
    then
        echo export "$v='$value'" >> "/home/rove/Projects/tfs/cmake-build-release/deactivate_conanrunenv-release-x86_64.sh"
    else
        echo unset $v >> "/home/rove/Projects/tfs/cmake-build-release/deactivate_conanrunenv-release-x86_64.sh"
    fi
done

