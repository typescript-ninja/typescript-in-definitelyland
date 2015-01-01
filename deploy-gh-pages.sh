#!/bin/bash -eu

if [ "$CIRCLECI" != "true" ]; then
  echo "not on Circle CI!"
  exit 1
fi
if [ "$CIRCLE_BRANCH" != "master" ]; then
  echo "not in master branch"
  exit 1
fi

# GITHUB_TOKEN はCircle CIの画面から登録(ないしょ)
# clone
git clone --quiet https://${GITHUB_TOKEN}@github.com/${CIRCLE_PROJECT_USERNAME}/${CIRCLE_PROJECT_REPONAME}.git pages
cd pages
git checkout -b gh-pages origin/gh-pages

# update
rm -rf *.html *.css images/
cp -r ../public/** ./

# commit & push
git add .
git commit -a -m "build ${CIRCLE_BUILD_NUM} commit:${CIRCLE_SHA1}"
git push --quiet https://${GITHUB_TOKEN}@github.com/${CIRCLE_PROJECT_USERNAME}/${CIRCLE_PROJECT_REPONAME}.git gh-pages
