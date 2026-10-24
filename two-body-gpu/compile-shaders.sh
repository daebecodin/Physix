#!/bin/sh
set -eu

# Run from any directory. Pass SPIRV, DXIL, or MSL to compile one format.
here=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
format=${1:-all}
command -v shadercross >/dev/null 2>&1 || {
    echo "shadercross is required (SDL_shadercross CLI)" >&2
    exit 1
}

compile_format() {
    target=$1
    mkdir -p "$here/shaders/$target"
    for source in "$here"/shaders/source/*.hlsl; do
        name=$(basename "$source" .hlsl)
        case "$target" in
            SPIRV) extension=spv ;;
            DXIL) extension=dxil ;;
            MSL) extension=msl ;;
        esac
        shadercross "$source" -o "$here/shaders/$target/$name.$extension"
    done
}

case "$format" in
    all) compile_format SPIRV; compile_format DXIL; compile_format MSL ;;
    SPIRV|DXIL|MSL) compile_format "$format" ;;
    *) echo "Usage: $0 [SPIRV|DXIL|MSL]" >&2; exit 2 ;;
esac
