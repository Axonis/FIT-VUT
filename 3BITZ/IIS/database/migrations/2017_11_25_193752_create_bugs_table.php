<?php

use Illuminate\Support\Facades\Schema;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Database\Migrations\Migration;

class CreateBugsTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {

        Schema::create('bugs', function (Blueprint $table) {
            $table->increments('id_bug');
            $table->string('typ');
            $table->string('zavaznost');
            $table->string('popis_bugu');
            $table->unsignedInteger('id_modul');
            $table->unsignedInteger('id_patch')->nullable();
            $table->foreign('id_modul')->references('id_modul')->on('modules');
        });

    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        Schema::dropIfExists('bugs');
    }
}
