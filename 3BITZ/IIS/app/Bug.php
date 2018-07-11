<?php

namespace App;

use Illuminate\Database\Eloquent\Model;

class Bug extends Model
{
    public static function new_bugs () {
        return static::where('zavaznost', 'High')->get();
    }
}
