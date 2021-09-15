#repo=openshift/origin
repo=library/nginx
token=$(curl -sSL "https://auth.docker.io/token?service=registry.docker.io&scope=repository:${repo}:pull" | jq --raw-output .token)
curl -i -H "Authorization: Bearer $token" "https://registry-1.docker.io/v2/${repo}/manifests/latest"
