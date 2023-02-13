echo "echo Restoring environment" > "/projects/tfs/cmake-build-release/deactivate_conanrunenv-release-x86_64.sh"
for v in 
do
    is_defined="true"
    value=$(printenv $v) || is_defined="" || true
    if [ -n "$value" ] || [ -n "$is_defined" ]
    then
        echo export "$v='$value'" >> "/projects/tfs/cmake-build-release/deactivate_conanrunenv-release-x86_64.sh"
    else
        echo unset $v >> "/projects/tfs/cmake-build-release/deactivate_conanrunenv-release-x86_64.sh"
    fi
done

