std = {
  globals = {
    table = {
      fields = {"concat", "contains", "find", "insert", "maxn", "pack", "remove", "sort", "unpack"},
    },
  },
}

ignore = {
  "111", -- setting non-standard global variable
  "631", -- line is too long
}

files["data/**"].ignore = {
  "113", -- accessing undefined variable
}
