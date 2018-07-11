<?php

namespace App\Http\Controllers;

use App\Bug;
use App\Patch;
use DB;
use App\Test;
use Illuminate\Http\Request;

class TestController extends Controller
{
    public function index(){
        $tests = Test::all();
        return view('tests.tests', compact('tests'));
    }

    public function show($id_test){
        $test = Test::where('id', $id_test)->first();
        return view('tests.test', compact('test'));
    }

    public function store(Request $request){
        $validate = $request->validate([
            'result' => 'required',
        ]);

        if ($request->approve){
            DB::table('tests')->where('id', $request->id)->update([
                'name_schvaleny' => $request->approve,
            ]);
        } else {
            DB::table('tests')->where('id', $request->id)->update([
                'datum_zaciatku' => $request->start_date,
                'datum_konca' => $request->end_date,
                'hodnotenie' => $request->result,
            ]);
        }
        $test = Test::where('id', $request->id)->first();
        return view('tests.test', compact('test'));
    }

    public function create_show(){
        $patches = DB::table('patches')->where('schvalenie', '0')->get();

        $data = [];

        foreach ($patches as $patch){
            $bugs = Bug::where('id_patch', $patch->id_patch)->get();
            $bugs_str ='';
            foreach ($bugs as $bug){
                if (array($bug) === last($bugs)) {
                    $bugs_str .= $bug->id_bug;
                } else {
                    $bugs_str .= $bug->id_bug . ', ';
                }

            }
            array_push($data,[$patch, $bugs_str]);
        }

        return view('tests.create', compact('data'));
    }

    public function create_store(Request $request){
        DB::table('tests')->insert([
            'napisal' => $request->user,
            'code' => $request->code,
            'id_patch' => $request->id_patch,
        ]);

        $test = Test::all()->last();
        $route = '/tests/'.$test->id;
        return redirect($route);
    }
}
