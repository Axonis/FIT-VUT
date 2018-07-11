<?php

namespace App\Http\Controllers;

use App\DataTypes;
use DB;
use App\User;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;

class ProfileController extends Controller
{
    public function index() {
        $data_lang = DataTypes::$languages;

        $languages = DB::table('users')
            ->where('name', Auth::user()->name)
            ->join('know', 'users.name', '=', 'know.user')
            ->get()->pluck('id_lang')->toArray();

        $lang_array = [];
        foreach ($languages as $item) {
            $name = DB::table('languages')
                ->where('id', $item)
                ->get()->pluck('name')->toArray();
            $name = implode('', $name);
            array_push($lang_array, $name);
        }

        $inter = array_intersect($data_lang, $lang_array);
        $diff = array_diff($data_lang, $lang_array);

        return view('profile.profile', compact('inter', 'diff'));
    }

    public function store(Request $request) {

        DB::table('know')->where('user', Auth::user()->name)->delete();

        if($request->langs) {
            foreach ($request->langs as $item) {
                $lang = DB::table('languages')->where('name', $item)->get()->pluck('id')->toArray();
                $num = implode('', $lang);
                DB::table('know')->insert([
                    'user' => Auth::user()->name,
                    'id_lang' => $num,
                ]);
            }
        }



        DB::table('users')->where('name', $request->name)->update([
            'name' => $request->name,
            'vek' => $request->vek,
            'cele_meno' => $request->cele_meno,
            'email' => $request->email,
        ]);


        return redirect('profile');
    }

    public function show($name) {
        $user = User::where('name', $name)->first();
        $languages = DB::table('users')
            ->where('name', Auth::user()->name)
            ->join('know', 'users.name', '=', 'know.user')
            ->get()->pluck('id_lang')->toArray();

        $lang_array = [];
        foreach ($languages as $item) {
            $name = DB::table('languages')
                ->where('id', $item)
                ->get()->pluck('name')->toArray();
            $name = implode('', $name);
            array_push($lang_array, $name);
        }
        $data = implode(', ', $lang_array);
        return view('profile.user', compact('user', 'data'));
    }
}
